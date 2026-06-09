# -*- mode: python ; coding: utf-8 -*-

import os

block_cipher = None
project_dir = os.path.abspath(os.getcwd())

a = Analysis(
    ['main.py'],
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
    hooksconfig={},
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
    name='Memory Viewer ODB',
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
    icon=r'Ressources\icon.ico',
)
