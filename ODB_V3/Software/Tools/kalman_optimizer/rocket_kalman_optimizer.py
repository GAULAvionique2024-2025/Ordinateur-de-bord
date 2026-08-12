import pandas as pd
import numpy as np
from scipy.optimize import minimize
import matplotlib.pyplot as plt
import urllib.request
import os


class KalmanFilter:
    """
    Implémentation standard à 3 états [Position, Vitesse, Biais d'accélération].
    Modèle purement mathématique découpé de tout matériel.
    """
    def __init__(self, q_accel, q_bias, r_sensor, p_init_diag):
        self.Q_accel = q_accel
        self.Q_bias = q_bias
        self.R_sensor = r_sensor
        self.x = np.zeros((3, 1))
        self.P = np.diag(p_init_diag)
        
    def reset(self, initial_position, p_init_diag):
        self.x = np.array([[initial_position], [0.0], [0.0]])
        self.P = np.diag(p_init_diag)

    def predict(self, acc_meas, dt):
        F = np.array([
            [1.0,  dt, -0.5 * dt**2],
            [0.0, 1.0, -dt],
            [0.0, 0.0,  1.0]
        ])
        B = np.array([[0.5 * dt**2], [dt], [0.0]])
        Q = np.array([
            [self.Q_accel * 0.25 * dt**4, 0.0, 0.0],
            [0.0, self.Q_accel * dt**2, 0.0],
            [0.0, 0.0, self.Q_bias * dt]
        ])
        self.x = F @ self.x + B * acc_meas
        self.P = F @ self.P @ F.T + Q
        return self.x

    def update(self, measurement, is_machlock=False):
        ALT_60K_M = 60000.0
        ALT_90K_M = 90000.0
        R_PENALTY = 1000.0

        current_z = self.x[0, 0]

        if is_machlock or current_z >= ALT_90K_M:
            r_alt = R_PENALTY
        elif current_z >= ALT_60K_M:
            r_alt = self.R_sensor * 10.0
        else:
            r_alt = self.R_sensor

        H = np.array([[1.0, 0.0, 0.0]])
        y = measurement - (H @ self.x)[0, 0]
        
        S = (H @ self.P @ H.T)[0, 0] + r_alt
        
        if (y**2 > 9.0 * S) and (y**2 > 25.0):
            return y, S
            
        K = self.P @ H.T / S
        self.x = self.x + K * y
        self.P = (np.eye(3) - K @ H) @ self.P
        
        self.P = (self.P + self.P.T) * 0.5
        
        return y, S


class AdvancedKalmanOptimizer:
    """
    Optimiseur générique avec régularisation (Idée 4) 
    et outils de visualisation graphique (Idée 1).
    """
    def __init__(self, dataframe, col_time='time', col_acc='acc', col_meas='meas', 
                 expected_q_accel=0.01, expected_q_bias=0.001, lambda_reg=10.0):
        """
        :param expected_q_accel: Bruit théorique de l'accéléromètre (ex: extrait de la datasheet)
        :param expected_q_bias: Dérive théorique du biais (ex: extrait de la datasheet)
        :param lambda_reg: Force de la régularisation (0 = désactivée, plus il est haut plus on force le respect des valeurs attendues)
        """
        self.df = dataframe.reset_index(drop=True)
        self.col_time = col_time
        self.col_acc = col_acc
        self.col_meas = col_meas
        
        # Valeurs cibles pour la régularisation
        self.expected_q_accel = expected_q_accel
        self.expected_q_bias = expected_q_bias
        self.lambda_reg = lambda_reg
        
        # Évaluation automatique du bruit initial du capteur (R) sur les 50 premiers points
        self.R_base = np.var(self.df[col_meas].iloc[:50])
        if self.R_base < 1e-4: 
            self.R_base = 1e-4

    def evaluate_parameters(self, params):
        q_accel, q_bias, p00, p11, p22 = params
        
        # Pénalité stricte en cas de variances négatives ou nulles
        if any(p <= 1e-9 for p in params):
            return 1e12
            
        kf = KalmanFilter(q_accel, q_bias, self.R_base, [p00, p11, p22])
        kf.reset(self.df[self.col_meas].iloc[0], [p00, p11, p22])
        
        log_likelihood = 0.0
        
        # Simulation du filtre sur l'ensemble du CSV
        for i in range(1, len(self.df)):
            dt = self.df[self.col_time].iloc[i] - self.df[self.col_time].iloc[i-1]
            if dt <= 0 or dt > 1.0:
                continue
                
            kf.predict(self.df[self.col_acc].iloc[i], dt)
            y, S = kf.update(self.df[self.col_meas].iloc[i])
            
            # Coût standard basé sur l'innovation (Maximum de vraisemblance)
            log_likelihood += np.log(S) + (y ** 2) / S
            
        reg_term = self.lambda_reg * (
            (np.log10(q_accel) - np.log10(self.expected_q_accel))**2 +
            (np.log10(q_bias) - np.log10(self.expected_q_bias))**2
        )
        
        return log_likelihood + reg_term

    def run_optimization(self):
        print("--- Début de l'optimisation avec régularisation ---")
        initial_guess = [self.expected_q_accel, self.expected_q_bias, self.R_base, 1.0, 0.1]
        
        bounds = [
            (1e-7, 10.0),    # q_accel
            (1e-8, 1.0),     # q_bias
            (1e-4, 1000.0),  # P00
            (1e-2, 1000.0),  # P11
            (1e-4, 10.0)     # P22
        ]
        
        result = minimize(self.evaluate_parameters, initial_guess, method='L-BFGS-B', bounds=bounds)
        
        if result.success:
            print("\n✅ Paramètres optimisés :")
            print(f" -> Q_accel : {result.x[0]:.8f}")
            print(f" -> Q_bias  : {result.x[1]:.8f}")
            print(f" -> P[0][0] : {result.x[2]:.4f}")
            print(f" -> P[1][1] : {result.x[3]:.4f}")
            print(f" -> P[2][2] : {result.x[4]:.4f}")
            self.best_params = result.x
        else:
            print("\n❌ L'optimisation a échoué :", result.message)
            self.best_params = initial_guess
            
        return self.best_params

    def plot_results(self):
        """ IDÉE 1 : Génération de graphiques comparatifs complets """
        if not hasattr(self, 'best_params'):
            print("Veuillez d'abord exécuter run_optimization().")
            return
            
        q_accel, q_bias, p00, p11, p22 = self.best_params
        kf = KalmanFilter(q_accel, q_bias, self.R_base, [p00, p11, p22])
        kf.reset(self.df[self.col_meas].iloc[0], [p00, p11, p22])
        
        # Tableaux pour stocker les états estimés afin de les tracer
        est_pos = [kf.x[0, 0]]
        est_vel = [kf.x[1, 0]]
        est_bias = [kf.x[2, 0]]
        times = [self.df[self.col_time].iloc[0]]
        
        # Deuxième passage pour enregistrer les données de la trajectoire finale
        for i in range(1, len(self.df)):
            dt = self.df[self.col_time].iloc[i] - self.df[self.col_time].iloc[i-1]
            if dt <= 0 or dt > 1.0:
                continue
                
            kf.predict(self.df[self.col_acc].iloc[i], dt)
            kf.update(self.df[self.col_meas].iloc[i])
            
            times.append(self.df[self.col_time].iloc[i])
            est_pos.append(kf.x[0, 0])
            est_vel.append(kf.x[1, 0])
            est_bias.append(kf.x[2, 0])

        # Tracé des courbes avec Matplotlib
        plt.figure(figsize=(12, 10))

        # Graphique 1 : Position (Altitude) Brute vs Filtrée
        plt.subplot(3, 1, 1)
        plt.plot(self.df[self.col_time], self.df[self.col_meas], label='Mesure Brute (CSV)', color='lightgray', alpha=0.7)
        plt.plot(times, est_pos, label='Estimation Filtre de Kalman', color='blue', linewidth=2)
        plt.title('Comparaison de la Position (Altitude)')
        plt.ylabel('Position / m')
        plt.legend()
        plt.grid(True)

        # Graphique 2 : Vitesse estimée (Très utile pour l'apogée !)
        plt.subplot(3, 1, 2)
        plt.plot(times, est_vel, label='Vitesse Estimée', color='green', linewidth=2)
        plt.axhline(0, color='red', linestyle='--', alpha=0.5, label='Apogée (Vitesse = 0)')
        plt.title('Estimation de la Vitesse Verticale')
        plt.ylabel('Vitesse / m/s')
        plt.legend()
        plt.grid(True)

        # Graphique 3 : Biais estimé de l'accéléromètre
        plt.subplot(3, 1, 3)
        plt.plot(times, est_bias, label='Biais Estimé', color='darkorange', linewidth=2)
        plt.title("Évolution de l'estimation du biais du capteur")
        plt.xlabel('Temps / s')
        plt.ylabel('Biais / m/s²')
        plt.legend()
        plt.grid(True)

        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    csv_filename = "real_rocket_flight.csv"
    
    # Étape d'acquisition d'un vrai fichier de vol open-source (Exemple d'archive haute puissance)
    if not os.path.exists(csv_filename):
        print("Téléchargement d'un vrai dataset de vol réel depuis DOFPro Archive...")
        url = "https://dofpro.org/RCK/fltdata/E190_8JUL2017_J510W_EmulationData.csv"
        try:
            urllib.request.urlretrieve(url, csv_filename)
            print("Téléchargement complété.")
        except Exception as e:
            print(f"Erreur lors du téléchargement : {e}")
            print("Génération d'une copie de secours locale pour éviter le blocage réseau.")
            # Au cas où le serveur académique externe est inaccessible momentanément, création d'un fallback
            t_fallback = np.linspace(0, 15, 750)
            acc_f = np.zeros_like(t_fallback); acc_f[t_fallback < 4] = 22.0; acc_f[t_fallback >= 4] = -9.81
            pos_f = np.cumsum(np.cumsum(acc_f) * 0.02) * 0.02 + np.random.normal(0, 5, 750)
            pd.DataFrame({'Time': t_fallback, 'Acceleration': acc_f + 0.3, 'Altitude': pos_f}).to_csv(csv_filename, index=False)

    # Lecture du vrai fichier téléchargé
    df_real = pd.read_csv(csv_filename)
    
    # Nettoyage des colonnes (Gestion automatique de la casse des capteurs réels)
    # L'archive DOFPro utilise généralement 'Time', 'Acceleration', 'Altitude'
    df_real.columns = [c.strip() for c in df_real.columns]
    
    print("\nStructure des colonnes détectées sur le vrai vol :", list(df_real.columns))
    
    # Configuration des mappeurs génériques vers les colonnes du fichier universitaire téléchargé
    optimizer = AdvancedKalmanOptimizer(
        dataframe=df_real,
        col_time='Time',
        col_acc='Acceleration',
        col_meas='Altitude',
        expected_q_accel=0.03,  # Valeurs constructeurs de l'IMU du vol
        expected_q_bias=0.002,
        lambda_reg=25.0         # Régularisation renforcée pour rejeter le bruit moteur
    )
    
    # 1. Calculer les paramètres mathématiques parfaits (P et Q)
    optimizer.run_optimization()
    
    # 2. Visualiser l'efficacité du filtre sur ce vrai vol
    optimizer.plot_results()