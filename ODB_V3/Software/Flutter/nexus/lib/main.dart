import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:flutter_web_plugins/url_strategy.dart';
import '/flutter_flow/flutter_flow_theme.dart';
import 'flutter_flow/flutter_flow_util.dart';
import 'flutter_flow/nav/nav.dart';
import 'index.dart';
import 'package:provider/provider.dart';
import 'services/bluetooth_service.dart';
import 'services/data_service.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  GoRouter.optionURLReflectsImperativeAPIs = true;
  usePathUrlStrategy();

  await FlutterFlowTheme.initialize();

  runApp(
    MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => BluetoothServiceManager()),
        ChangeNotifierProxyProvider<BluetoothServiceManager, DataServiceManager>(
          create: (ctx) => DataServiceManager(ctx.read<BluetoothServiceManager>()),
          update: (ctx, bt, prev) => prev ?? DataServiceManager(bt),
        ),
      ],
      child: MyApp(),
    ),
  );
}

class MyApp extends StatefulWidget {
  // This widget is the root of your application.
  @override
  State<MyApp> createState() => _MyAppState();

  static _MyAppState of(BuildContext context) =>
      context.findAncestorStateOfType<_MyAppState>()!;
}

class _MyAppState extends State<MyApp> {
  ThemeMode _themeMode = FlutterFlowTheme.themeMode;

  late AppStateNotifier _appStateNotifier;
  late GoRouter _router;
  String getRoute([RouteMatch? routeMatch]) {
    final RouteMatch lastMatch =
        routeMatch ?? _router.routerDelegate.currentConfiguration.last;
    final RouteMatchList matchList = lastMatch is ImperativeRouteMatch
        ? lastMatch.matches
        : _router.routerDelegate.currentConfiguration;
    return matchList.uri.toString();
  }

  List<String> getRouteStack() =>
      _router.routerDelegate.currentConfiguration.matches
          .map((e) => getRoute(e))
          .toList();
  @override
  void initState() {
    super.initState();

    _appStateNotifier = AppStateNotifier.instance;
    _router = createRouter(_appStateNotifier);
  }

  void setThemeMode(ThemeMode mode) => safeSetState(() {
        _themeMode = mode;
        FlutterFlowTheme.saveThemeMode(mode);
      });

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      debugShowCheckedModeBanner: false,
      title: 'Nexus',
      localizationsDelegates: const [
        GlobalMaterialLocalizations.delegate,
        GlobalWidgetsLocalizations.delegate,
        GlobalCupertinoLocalizations.delegate,
      ],
      supportedLocales: const [Locale('fr', '')],
      theme: ThemeData(
        brightness: Brightness.light,
      ),
      darkTheme: ThemeData(
        brightness: Brightness.dark,
      ),
      themeMode: _themeMode,
      routerConfig: _router,
    );
  }
}

class NavBarPage extends StatefulWidget {
  const NavBarPage({
    Key? key,
    this.initialPage,
    this.page,
    this.disableResizeToAvoidBottomInset = false,
  }) : super(key: key);

  final String? initialPage;
  final Widget? page;
  final bool disableResizeToAvoidBottomInset;

  @override
  _NavBarPageState createState() => _NavBarPageState();
}

/// This is the private State class that goes with NavBarPage.
class _NavBarPageState extends State<NavBarPage> {
  String _currentPageName = 'OverviewPage';
  late Widget? _currentPage;

  // Transitio bidirectionnel
  int _previousIndex = 0;
  int _currentIndex = 0;

  @override
  void initState() {
    super.initState();
    _currentPageName = widget.initialPage ?? _currentPageName;
    _currentPage = widget.page;
  }

  @override
  Widget build(BuildContext context) {
    final tabs = {
      'OverviewPage': const OverviewPageWidget(),
      'ConnectionPage': const ConnectionPageWidget(),
      'StatisticsPage': const StatisticsPageWidget(),
      'CommandsPage': const CommandsPageWidget(),
      'RSSIPage': const RSSIPageWidget(),
    };
    _currentIndex = tabs.keys.toList().indexOf(_currentPageName);

    return Scaffold(
      resizeToAvoidBottomInset: !widget.disableResizeToAvoidBottomInset,
      body: AnimatedSwitcher(
        duration: const Duration(milliseconds: 300),
        switchInCurve: Curves.easeOutCubic,
        switchOutCurve: Curves.easeInCubic,
        transitionBuilder: (child, animation) {
          final isForward = _currentIndex >= _previousIndex;

          final beginOffset = isForward
              ? const Offset(1.0, 0.0)   // slide depuis la droite
              : const Offset(-1.0, 0.0); // slide depuis la gauche

          return SlideTransition(
            position: Tween<Offset>(
              begin: beginOffset,
              end: Offset.zero,
            ).animate(animation),
            child: child,
          );
        },
        child: KeyedSubtree(
          key: ValueKey(_currentPageName),
          child: _currentPage ?? tabs[_currentPageName]!,
        ),
      ),
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _currentIndex,
        onTap: (i) => safeSetState(() {
          _previousIndex = _currentIndex;
          _currentIndex = i;

          _currentPage = null;
          _currentPageName = tabs.keys.toList()[i];
        }),
        backgroundColor: FlutterFlowTheme.of(context).primaryBackground,
        selectedItemColor: FlutterFlowTheme.of(context).primary,
        unselectedItemColor: FlutterFlowTheme.of(context).secondaryText,
        showSelectedLabels: false,
        showUnselectedLabels: false,
        type: BottomNavigationBarType.fixed,
        items: const <BottomNavigationBarItem>[
          BottomNavigationBarItem(
            icon: Icon(
              Icons.home_outlined,
              size: 24.0,
            ),
            activeIcon: Icon(
              Icons.home,
              size: 24.0,
            ),
            label: 'Overview',
            tooltip: '',
          ),
          BottomNavigationBarItem(
            icon: Icon(
              Icons.bluetooth_drive_outlined,
              size: 24.0,
            ),
            activeIcon: Icon(
              Icons.bluetooth_drive,
              size: 24.0,
            ),
            label: 'Connection',
            tooltip: '',
          ),
          BottomNavigationBarItem(
            icon: Icon(
              Icons.assessment_outlined,
              size: 24.0,
            ),
            activeIcon: Icon(
              Icons.assessment,
              size: 24.0,
            ),
            label: 'Statistics',
            tooltip: '',
          ),
          BottomNavigationBarItem(
            icon: Icon(
              Icons.keyboard_command_key_outlined,
              size: 24.0,
            ),
            activeIcon: Icon(
              Icons.keyboard_command_key,
              size: 24.0,
            ),
            label: 'Commands',
            tooltip: '',
          ),
          BottomNavigationBarItem(
            icon: Icon(
              Icons.compass_calibration_outlined,
              size: 24.0,
            ),
            activeIcon: Icon(
              Icons.compass_calibration,
              size: 24.0,
            ),
            label: 'RSSI',
            tooltip: '',
          )
        ],
      ),
    );
  }
}
