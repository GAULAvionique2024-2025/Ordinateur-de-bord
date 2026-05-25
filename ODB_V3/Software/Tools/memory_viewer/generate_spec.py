#!/usr/bin/env python3
import os
import sys

def create_spec_file(script_name, icon_path):
    """
    Génère un fichier .spec pour PyInstaller avec une icône.
    """
    base_name = os.path.splitext(script_name)[0]
    app_name = "Memory Viewer ODB"
    spec_content = f"""# -*- mode: python ; coding: utf-8 -*-

import os

block_cipher = None
project_dir = os.path.abspath(os.getcwd())

a = Analysis(
    ['{script_name}'],
    pathex=[project_dir],
    binaries=[],
    datas=[
        (os.path.join(project_dir, 'Ressources', 'style.qss'), 'Ressources'),
        (os.path.join(project_dir, 'Ressources', 'echarts.min.js'), 'Ressources'),
        (os.path.join(project_dir, 'Ressources', 'three.min.js'), 'Ressources'),
        (os.path.join(project_dir, 'Ressources', 'icon.ico'), 'Ressources'),
    ],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={{}},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)
pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='{app_name}',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,  # Mettre False pour une application GUI sans console
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=r'{icon_path}',
)
"""
    with open(f"{base_name}.spec", "w") as f:
        f.write(spec_content)
    print(f"Fichier {base_name}.spec généré avec succès.")
    print(f"Vous pouvez maintenant exécuter: pyinstaller {base_name}.spec")

if __name__ == "__main__":
    project_dir = os.path.dirname(os.path.abspath(__file__))
    script_name = sys.argv[1] if len(sys.argv) >= 2 and sys.argv[1].lower().endswith('.py') else 'main.py'
    icon_path = sys.argv[2] if len(sys.argv) >= 3 and sys.argv[2].lower().endswith('.ico') else os.path.join('Ressources', 'icon.ico')

    if len(sys.argv) > 3:
        print("Usage: python generate_spec.py [mon_script.py] [mon_icone.ico]")
    else:
        create_spec_file(script_name, icon_path)