<?php

function janrain_user_reset_tables() {
  db_truncate('janrain_user_forms')->execute();
  db_truncate('janrain_user_fields')->execute();
  db_truncate('janrain_user_form_fields')->execute();
}

function janrain_user_get_forms_from_db() {
  $records = db_select('janrain_user_forms', 'juf')
    ->fields('juf')
    ->execute()
    ->fetchAll();

  $ju_forms = array();

  foreach ($records as $record) {
    $ju_forms[$record->ju_form_id] = $record->ju_form_name;
  }

  return $ju_forms;
}

function janrain_user_get_existing_fields() {
  $records = db_select('janrain_user_fields', 'juf')
    ->fields('juf')
    ->execute()
    ->fetchAll();

  $ju_fields = array();

  foreach ($records as $record) {
    $ju_fields[$record->ju_field_id] = $record->ju_field_name;
  }

  return $ju_fields;
}

function janrain_user_get_form_fields($form_drupal_id) {
  $query = db_select('janrain_user_form_fields', 'juff');
  $query->join('janrain_user_fields', 'juf', 'juff.ju_field_id_fk = juf.ju_field_id');
  $query->fields('juf')
    ->condition('juff.ju_form_id_fk', $form_drupal_id);
  $records = $query->execute()
    ->fetchAllAssoc('ju_field_id', PDO::FETCH_ASSOC);

  return $records;
}

function janrain_user_load_form_by_name($form_name) {
  $query = db_select('janrain_user_forms', 'jufo');
  $query->join('janrain_user_form_fields', 'juff', 'jufo.ju_form_id = juff.ju_form_id_fk');
  $query->join('janrain_user_fields', 'jufi', 'jufi.ju_field_id = juff.ju_field_id_fk');
  $query->fields('jufi')
    ->condition('jufo.ju_form_name', $form_name);
  $records = $query->execute()
    ->fetchAll();

  print_r($records);die;

}
