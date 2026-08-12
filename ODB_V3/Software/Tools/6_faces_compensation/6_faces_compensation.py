import numpy as np
from scipy.optimize import minimize

# ==============================================================================
# 1. ENTRÉE DES DONNÉES DES 6 FACES (EN m/s²)
# ==============================================================================
mesures_brutes = np.array([
    [ 0.147,  0.019,  10.002], # Face 1: Z vers le ciel (+9.8 m/s²)
    [-0.117, -0.009, -9.708],  # Face 2: Z vers le sol (-9.8 m/s²)
    [ 0.049,  9.904,  0.176],  # Face 3: Y vers le ciel (+9.8 m/s²)
    [ 0.009, -9.659,  0.107],  # Face 4: Y vers le sol (-9.8 m/s²)
    [ 9.953,  0.078,  0.049],  # Face 5: X vers le ciel (+9.8 m/s²)
    [-9.757, -0.029, -0.019]   # Face 6: X vers le sol (-9.8 m/s²)
])

# Constante de gravité terrestre
GRAVITE_CIBLE = 9.80665

# ==============================================================================
# 2. FONCTION DE COÛT
# ==============================================================================
def cost_function(params, measurements):
    M = params[0:9].reshape((3, 3))
    B = params[9:12]
    
    erreur_totale = 0.0
    for acc in measurements:
        acc_corr = M @ (acc - B)
        norme = np.linalg.norm(acc_corr)
        erreur_totale += (norme - GRAVITE_CIBLE)**2
        
    return erreur_totale

# ==============================================================================
# 3. OPTIMISATION
# ==============================================================================
initial_guess = np.array([1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0])

print("--- Début de l'optimisation 6-faces (m/s²) ---")
resultat = minimize(cost_function, initial_guess, args=(mesures_brutes,), method='BFGS')

if resultat.success:
    M_opt = resultat.x[0:9].reshape((3, 3))
    B_opt = resultat.x[9:12]
    
    print("\n✅ Optimisation réussie !\n")
    
    c_code = f"""// Vecteur de biais statique (B) en m/s2
const float IMU_BIAS[3] = {{
    {B_opt[0]:.6f}f, {B_opt[1]:.6f}f, {B_opt[2]:.6f}f
}};

// Matrice d'échelle et d'orthogonalité (M)
const float IMU_SCALE_MATRIX[3][3] = {{
    {{{M_opt[0,0]:.6f}f, {M_opt[0,1]:.6f}f, {M_opt[0,2]:.6f}f}},
    {{{M_opt[1,0]:.6f}f, {M_opt[1,1]:.6f}f, {M_opt[1,2]:.6f}f}},
    {{{M_opt[2,0]:.6f}f, {M_opt[2,1]:.6f}f, {M_opt[2,2]:.6f}f}}
}};
"""
    print(c_code)
    
    print("\n--- Vérification des normes (Cible : 9.80665 m/s²) ---")
    for i, acc in enumerate(mesures_brutes):
        acc_corr = M_opt @ (acc - B_opt)
        print(f"Face {i+1} : Norme = {np.linalg.norm(acc_corr):.5f} m/s²")
else:
    print("\n❌ Échec de l'optimisation :", resultat.message)