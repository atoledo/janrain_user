<?php

function janrain_user_field_batch($field_drupal_key, $field_name, $settings, &$context) {
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.aux');
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.capi');
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.db');
  $flow = variable_get('janrain_user_flow_name');
  $ju_forms = janrain_user_get_forms_from_db();

  $capi = new JanrainCAPI($settings);
  $field_result = $capi->getFlowField($flow, $field_name);

  $record = array(
    'ju_field_id' => $field_drupal_key,
    'ju_field_name' => $field_result->name,
    'ju_field_type' => $field_result->type,
    'ju_field_required' => janrain_user_field_is_required($field_result),
  );
  drupal_write_record('janrain_user_fields', $record, array('ju_field_id'));

  foreach ($field_result->_relationships->forms as $relationship_form) {
    $record = array(
      'ju_form_id_fk' => array_search($relationship_form->name, $ju_forms),
      'ju_field_id_fk' => $field_drupal_key,
    );
    drupal_write_record('janrain_user_form_fields', $record);
  }
}

function janrain_user_field_batch_finished($success, $results) {
  drupal_set_message(t('Field definitions loaded successfully'));
}
