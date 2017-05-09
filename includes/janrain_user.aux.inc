<?php

function janrain_user_field_is_required($field) {
  if (isset($field->validation)) {
    foreach ($field->validation as $validation) {
      if ($validation->rule == 'required') {
        return $validation->value;
      }
    }
  }

  return 0;
}
