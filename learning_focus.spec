import sys

ana = Analysis(
	['learning_focus.py'],
	pathex = [],
	binaries = [
		(
			sys.path[5] + '\\face_recognition_models\\models\\*.*',
			'.\\face_recognition_models\\models'
		)
	],
	datas = [],
	hiddenimports = [],
	hookspath = [],
	hooksconfig = {},
	runtime_hooks = [],
	excludes = [],
	win_no_prefer_redirects = False,
	win_private_assemblies = False,
	cipher = None,
	noarchive = False,
)

pyz = PYZ(ana.pure, ana.zipped_data, cipher = None)

exe = EXE(
	pyz,
	ana.scripts,
	ana.binaries,
	ana.zipfiles,
	ana.datas,
	[],
	name = 'learning_focus',
	debug = False,
	bootloader_ignore_signals = False,
	strip = False,
	upx = False,
	upx_exclude = [],
	runtime_tmpdir = None,
	console = True,
	disable_windowed_traceback = False,
	argv_emulation = False,
	target_arch = None,
	codesign_identity = None,
	entitlements_file = None,
	icon = 'learning_focus.ico'
)