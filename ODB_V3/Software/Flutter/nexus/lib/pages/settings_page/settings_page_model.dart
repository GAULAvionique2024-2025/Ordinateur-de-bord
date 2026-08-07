import '/flutter_flow/flutter_flow_util.dart';
import 'settings_page_widget.dart' show SettingsPageWidget;
import 'package:expandable/expandable.dart';
import 'package:flutter/material.dart';

class SettingsPageModel extends FlutterFlowModel<SettingsPageWidget> {
  ///  State fields for stateful widgets in this page.

  // State field(s) for Expandable widget.
  late ExpandableController expandableExpandableController;
  late ExpandableController resetExpandableController;
  late ExpandableController sensorsExpandableController;
  late ExpandableController pyrosExpandableController;
  late ExpandableController phaseExpandableController;
  late ExpandableController parachuteExpandableController;
  late ExpandableController idefixExpandableController;
  late ExpandableController audioExpandableController;
  late ExpandableController aProposExpandableController;

  @override
  void initState(BuildContext context) {}

  @override
  void dispose() {
    expandableExpandableController.dispose();
    resetExpandableController.dispose();
    sensorsExpandableController.dispose();
    pyrosExpandableController.dispose();
    phaseExpandableController.dispose();
    parachuteExpandableController.dispose();
    idefixExpandableController.dispose();
    audioExpandableController.dispose();
    aProposExpandableController.dispose();
  }
}
