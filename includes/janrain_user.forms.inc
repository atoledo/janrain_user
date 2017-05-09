<?php

function janrain_user_temp_registration_form() {
  $form = janrain_user_build_form('registrationForm');

  return $form;
}

function janrain_user_build_form($janrain_form_name) {
  module_load_include('inc', 'janrain_user', 'includes/janrain_user.db');

  $ju_fields = janrain_user_load_form_by_name('registrationForm');

  foreach ($ju_fields as $field) {
    // TODO stopped here
  }
}
