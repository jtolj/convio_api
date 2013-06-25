<?php


function convio_api_admin_site_settings() {
  $form = array();
  
  $form['convio_api_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of organization'),
    '#description' => t('Please enter the site name'),
    '#default_value' => variable_get('convio_api_site_id', ''),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['convio_api_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('The base URL for the API'),
    '#description' => t('The base URL for the API calls. Different Convio customers have different base URLs (e.g., https://secure2.convio.net/ or https://secure3.convio.net/).'),
    '#default_value' => variable_get('convio_api_site_url', 'https://secure2.convio.net/'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  
  $form['convio_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('API key to use the Convio API'),
    '#default_value' => variable_get('convio_api_key', ''),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  
  $form['convio_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API Username'),
    '#description' => t('Special administrative account used just for API access'),
    '#default_value' => variable_get('convio_api_username', ''),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  
  $form['convio_api_password'] = array(
    '#type' => 'password',
    '#title' => t('API Password'),
    '#required' => TRUE,
  );

  $form['convio_api_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#required' => TRUE,
    '#description' => t('An e-mail address for a valid account within Convio (used to test connectivity).'),
    '#default_value' => variable_get('convio_api_email', ''),
  );
  
  $form = system_settings_form($form);
  return $form;
}

/**
 * Implements hook_form_validate
 */
function convio_api_admin_site_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!preg_match('/^https:\/\//', $values['convio_api_site_url']))
  {
    form_set_error('convio_api_site_url', 'Site URL *must* begin with https://');
    return; // We do not want to send our example request over http!
  }
  $params = array(
    'method' => 'isEmailValid',
    'login_name' => $values['convio_api_username'],
    'login_password' => $values['convio_api_password'],
    'api_key' => $values['convio_api_key'],
    'email' => $values['convio_api_email'],
  );
  $response = convio_api_request($params, 'constituent', 'server');
  if (!empty($response->error)) {
    form_set_error('', 'Could not connect to Convio API with the information provided. Error was: ' . $response->error);
  }
  else {
    drupal_set_message('Connection to Convio API was successful.');
  }
}
