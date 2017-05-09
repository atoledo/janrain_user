<?php

function janrain_user_get_forms_capi($settings) {
  $capi = new JanrainCAPI($settings);

  $forms_result = $capi->getForms($settings['flow']);

  foreach ($forms_result as $form) {
    $record = array('ju_form_name' => $form->name);
    drupal_write_record('janrain_user_forms', $record);
  }
}

function janrain_user_get_fields_capi($settings) {
  $capi = new JanrainCAPI($settings);

  $fields_result = $capi->getFields($settings['flow']);

  foreach ($fields_result as $field) {
    $record = array('ju_field_name' => $field->name);
    drupal_write_record('janrain_user_fields', $record);
  }
}
