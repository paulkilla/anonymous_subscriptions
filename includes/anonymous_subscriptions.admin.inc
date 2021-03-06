<?php

/**
 * Implements hook_settings().
 *
 * General Settings for Anonymous Subscriptions
 */
function anonymous_subscriptions_settings() {
  $form = array();

  $form['subscription_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription settings'),
  );

  $form['subscription_fieldset']['anonymous_subscriptions_verify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Emails must be verified'),
    '#default_value' => variable_get('anonymous_subscriptions_verify', FALSE),
  );

  $form['subscription_fieldset']['anonymous_subscriptions_send_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send by default'),
    '#default_value' => variable_get('anonymous_subscriptions_send_default', FALSE),
  );

  $form['subscription_fieldset']['anonymous_subscriptions_max_to_send'] = array(
    '#type' => 'select',
    '#title' => t('Maximum time to spend processing e-mails during cron.'),
    '#default_value' => variable_get('anonymous_subscriptions_max_to_send', 60),
    '#options' => array(30 => '30 seconds', 60 => '1 minute', 120 => '2 minutes'),
  );

  $form['subscription_fieldset']['anonymous_subscriptions_limit_window'] = array(
    '#type' => 'select',
    '#title' => t('How long to block flood attempts.'),
    '#default_value' => variable_get('anonymous_subscriptions_limit_window', 300),
    '#options' => array(60 => '1 minute', 300 => '5 minutes', 600 => '10 minutes'),
  );

  $form['subscription_fieldset']['anonymous_subscriptions_ip_limit'] = array(
    '#type' => 'select',
    '#title' => t('How many attempts before blocking a user from a single IP.'),
    '#default_value' => variable_get('anonymous_subscriptions_ip_limit', 20),
    '#options' => array(5 => '5 attempts', 10 => '10 attempts', 20 => '20 attempts'),
  );

  $form['email_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
  );

  $form['email_fieldset']['anonymous_subscriptions_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Site URL'),
    '#description' => t('The URL you want to use as the base URL sent in e-mails. Tokens available'),
    '#default_value' => variable_get('anonymous_subscriptions_site_url', '[site:url]'),
  );

  $form['email_fieldset']['anonymous_subscriptions_subject_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject text'),
    '#description' => 'Tokens are available',
    '#default_value' => variable_get('anonymous_subscriptions_subject_text', 'New content added to [site:name]'),
  );

  $form['email_fieldset']['anonymous_subscriptions_body_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Body text'),
    '#description' => 'Tokens are available',
    '#default_value' => variable_get('anonymous_subscriptions_body_text', "A new post has been made to [site:name].\nPlease visit [node:url] to see this new post!"),
  );

  if (module_exists('token')) {
    $form['email_fieldset']['token_help'] = array(
      '#theme' => 'token_tree_link',
      '#token_types' => array('site', 'node'),
    );
  }

  $form['anonymous_subscriptions_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types enabled for subscription notifications'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('anonymous_subscriptions_node_types', array()),
  );

  $form['array_filter'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_settings_validate().
 */
function anonymous_subscriptions_settings_validate($form, $form_state) {
  $url = $form['email_fieldset']['anonymous_subscriptions_site_url']['#value'];
  $url = token_replace($url);
  $url = parse_url($url);
  if(!isset($url) || empty($url) || !isset($url['scheme']) || empty($url['scheme']) ) {
    form_set_error('anonymous_subscriptions_site_url', t('Please insert a valid URL or Token.'));
  }
}

/**
 * Helper function to determine if a string starts with a
 * certain string
 *
 * @param $haystack
 * @param $needle
 * @return bool
 */
function _startsWith($haystack, $needle) {
  // search backwards starting from haystack length characters from the end
  return $needle === '' || strrpos($haystack, $needle, -strlen($haystack)) !== false;
}

/**
 * Helper function to determine if a string ends with a
 * certain string
 *
 * @param $haystack
 * @param $needle
 * @return bool
 */
function _endsWith($haystack, $needle) {
  // search forward starting from end minus needle length characters
  return $needle === '' || (($temp = strlen($haystack) - strlen($needle)) >= 0 && strpos($haystack, $needle, $temp) !== false);
}


