<?php
/**
 * @file
 * Janrain User admin file.
 */

function janrain_user_config_form($form, $form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Janrain API Settings'),
    '#description' => t('Settings for connecting to the Janrain API.'),
  );

  $form['settings']['janrain_user_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#description' => t('The Application ID.'),
    '#default_value' => variable_get('janrain_user_app_id'),
  );

  $form['settings']['janrain_user_capture_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture URL'),
    '#description' => t('The capture URL for your application.'),
    '#default_value' => variable_get('janrain_user_capture_url', 'https://domain.janraincapture.com'),
  );

  $form['settings']['janrain_user_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => t('The API client ID.'),
    '#default_value' => variable_get('janrain_user_client_id'),
  );

  $form['settings']['janrain_user_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#description' => t('The API client secret.'),
    '#default_value' => variable_get('janrain_user_client_secret'),
  );

  $form['settings']['janrain_user_flow_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Flow Name'),
    '#description' => t('Janrain Flow name.'),
    '#default_value' => variable_get('janrain_user_flow_name'),
  );

  $form['settings']['janrain_user_flow_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Flow Version'),
    '#description' => t('Janrain Flow version.'),
    '#default_value' => variable_get('janrain_user_flow_version'),
  );

  $form['settings']['janrain_user_entity_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity Type Name'),
    '#description' => t('Entity Type to save user data.'),
    '#default_value' => variable_get('janrain_user_entity_type'),
  );

  return system_settings_form($form);
}

function janrain_user_build_forms() {

  $form['janrain_user_markup'] = array(
    '#markup' => '<h3>' . t('The latest version of the flow will be used to rebuild the forms definitions.') . '</h3>',
  );

  $form['janrain_user_owner_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Owner Client ID'),
    '#default_value' => '',
  );

  $form['janrain_user_owner_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Owner Client Secret'),
    '#default_value' => '',
  );

  $form['janrain_user_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Janrain Forms'),
  );

  return $form;
}

function janrain_user_build_forms_submit($form, $form_state) {
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.aux');
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.capi');
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.db');

  $settings = array(
    'flow' => variable_get('janrain_user_flow_name'),
    'app_id' => variable_get('janrain_user_app_id'),
    'client_id' => $form_state['values']['janrain_user_owner_client_id'],
    'client_secret' => $form_state['values']['janrain_user_owner_client_secret'],
  );

  janrain_user_reset_tables();

  janrain_user_get_forms_capi($settings);
  janrain_user_get_fields_capi($settings);

  $batch = array(
    'finished' => 'janrain_user_field_batch_finished',
    'title' => t('Janrain Flow Fields batch'),
    'init_message' => t('Loading Janrain fields...'),
    'progress_message' => t('Processed @current out of @total fields.'),
    'error_message' => t('An error has been encountered.'),
    'file' => drupal_get_path('module', 'janrain_user') . '/includes/janrain_user.fields_batch.inc',
  );

  $ju_fields = janrain_user_get_existing_fields();

  $operations = array();
  foreach ($ju_fields as $field_drupal_key => $field_name) {
    $operations[] = array('janrain_user_field_batch', array($field_drupal_key, $field_name, $settings));
  }
  $batch['operations'] = $operations;

  batch_set($batch);
  batch_process('admin/config/people/janrain-user/available-forms');
}

function janrain_user_available_forms() {
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.db');

  $form = array();
  $header = array('id', 'name', 'type', 'required');

  $ju_forms = janrain_user_get_forms_from_db();

  if (!$ju_forms) {
    $form['markup'] = array('#markup' => t('No forms found in database.'));
  }
  else {
    foreach ($ju_forms as $form_drupal_key => $form_name) {
      $form_fields = (array) janrain_user_get_form_fields($form_drupal_key);

      $form['form_' . $form_name] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t($form_name . ' (' . count($form_fields) .')'),
      );

      $form['form_' . $form_name]['fields_' . $form_name] = array(
        '#markup' => theme('table', array('header' => $header, 'rows' => $form_fields)),
      );
    }
  }

  return $form;
}
