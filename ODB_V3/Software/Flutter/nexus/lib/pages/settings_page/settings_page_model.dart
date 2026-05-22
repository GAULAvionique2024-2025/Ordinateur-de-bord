import '/flutter_flow/flutter_flow_theme.dart';
import '/flutter_flow/flutter_flow_util.dart';
import '/flutter_flow/flutter_flow_widgets.dart';
import 'dart:ui';
import 'settings_page_widget.dart' show SettingsPageWidget;
import 'package:expandable/expandable.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:provider/provider.dart';

class SettingsPageModel extends FlutterFlowModel<SettingsPageWidget> {
  ///  State fields for stateful widgets in this page.

  // State field(s) for Expandable widget.
  late ExpandableController expandableExpandableController;
  late ExpandableController pyrosExpandableController;
  late ExpandableController phaseExpandableController;
  late ExpandableController parachuteExpandableController;
  late ExpandableController audioExpandableController;
  late ExpandableController aProposExpandableController;

  @override
  void initState(BuildContext context) {}

  @override
  void dispose() {
    expandableExpandableController.dispose();
    pyrosExpandableController.dispose();
    phaseExpandableController.dispose();
    parachuteExpandableController.dispose();
    audioExpandableController.dispose();
    aProposExpandableController.dispose();
  }
}
