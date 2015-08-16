<?php

/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 */
function hosting_saas_utils_post_hosting_verify_task($task, $data) {
  // TODO: Option to unset the settings configuration post-verify
  // (so the user can change them).

  $site = node_load($task->rid);

  if (!empty($site)) {
    $target = $site->hosting_name;
    $todo = variable_get('hosting_saas_utils_todo', array());

    if (!empty($todo[$site->hosting_name])) {
      $client_user = $todo[$site->hosting_name]['create_new_user'];

      if (!empty($client_user)) {
        drush_log(t('[hosting_saas_utils] Creating new user: !name (!email).', array('!name' => $client_user['name'], '!email' => $client_user['email'])));

        $results['user-create'] = provision_backend_invoke($target, 'user-create "' . $client_user['name'] . '" --mail="' . $client_user['email'] . '"');
      }
      else {
        $client_user =  $todo[$site->hosting_name]['change_admin_user'];

        if (!empty($client_user)) {
          //drush_log(t('[hosting_saas_utils] Assigning new values to admin user: !name (!email).', array('!name' => $client_user['name'], '!email' => $client_user['email'])));
          // TODO
        }
      }

      if (!empty($todo[$site->hosting_name]['set_user_role'])) {
        drush_log(t('[hosting_saas_utils] Assigning role: !role', array('!role' => $todo[$site->hosting_name]['set_user_role'])));
        $results['user-assign-role'] = provision_backend_invoke($target, "user-add-role '" . $todo[$site->hosting_name]['set_user_role'] . "' --mail='" . $client_user['email'] . "'");
      }

      //   send_email
      if ($todo[$site->hosting_name]['send_email']) {
        // Clumsily parse uid and send notification until Drush implements --notify  
        $user_info = provision_backend_invoke($target, 'user-information --full "' . $client_user['email'] . '"');

        if (preg_match ('/User ID.* ([0-9]+)/', $user_info['output'], $matches)) {
          $new_user_uid = $matches[1];
        }
        else {
          drush_set_error(t('[hosting_saas_utils] Could not parse user ID of new user! User creation failed?'));
        }

        drush_log('[hosting_saas_utils] Parsed uid of new user of new site: ' . $new_user_uid);

        $message = "status_activated";
        $result ['notify'] = provision_backend_invoke($target, "eval  '_user_mail_notify(\"".$message."\", user_load($new_user_uid));'");

        drush_log(t("[hosting_saas_utils] Sent registration email to !name",  array('!name' => $client_user['name'])));
      }

      unset($todo[$site->hosting_name]);
      variable_set('hosting_saas_utils_todo', $todo);
    }
  }
}
