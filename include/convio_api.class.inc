<?php

class ConvioAPI {
  private $api;
  private $api_data = array(
    'address_book' => array('path' => 'AddressBookAPI', 'supported_methods' => array('client')),
    'advocacy' => array('path' => 'AdvocacyAPI', 'supported_methods' => array('client', 'server')),
    'constituent' => array('path' => 'ConsAPI', 'supported_methods' => array('client', 'server')),
    'content' => array('path' => 'ContentAPI', 'supported_methods' => array('client', 'server')),
    'datasync' => array('path' => 'DataSyncAPI', 'supported_methods' => array('client', 'server')),
    'donation' => array('path' => 'DonationAPI', 'supported_methods' => array('client', 'server')),
    'event' => array('path' => 'OrgEventAPI', 'supported_methods' => array('client', 'server')),
    'group' => array('path' => 'GroupAPI', 'supported_methods' => array('client', 'server')),
    'single_sign_on' => array('path' => 'ConsAPI', 'supported_methods' => array('client', 'server')),
    'survey' => array('path' => 'SurveyAPI', 'supported_methods' => array('client')),
    'teamraiser' => array('path' => 'TeamraiserAPI', 'supported_methods' => array('client', 'server')),
  );
  private $config;
  private $method;
  private $params;
  private $request_url;
  
  public function __construct($api, $method, $config) {
    if (!empty($this->api_data[$api])) {
      $this->api = $api;
      if (in_array($method, $this->api_data[$api]['supported_methods'])) {
        $this->method = $method;
        $this->config = $config;
        $this->request_url = $this->getUrl();
      }
      else {
        throw new Exception("API $api does not support method $method");
      }
    }
    else {
      throw new Exception("API $api is not supported.");
    }
  }
  
  public function setParams($params) {
    $this->params = $params;
  }
  
  public function execute() {
    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded',
    );
    
    $default_params = array(
      'api_key' => $this->config['convio_api_key'],
      'v' => '1.0',
      'response_format' => 'json'
    );
  
    if ($this->method !== 'client') {
      $default_params += array(
        'login_name' => $this->config['convio_api_username'],
        'login_password' => $this->config['convio_api_password'],
      );
    }
    
    $params = array_merge($default_params, $this->params);
    
    return drupal_http_request($this->request_url, array('headers' => $headers, 'method' => 'POST', 'data' => http_build_query($params, '', '&')));
  }

  private function getUrl() {
    $method_path = ($this->method === 'client') ? 'CR' : 'SR';
    return $this->config['convio_api_site_url'] . $this->config['convio_api_site_id'] . '/site/' . $method_path . $this->api_data[$this->api]['path'];
  }
}