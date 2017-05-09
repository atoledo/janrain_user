<?php
/**
 * Janrain Configuration API Class
 * @author Andre Toledo
 */
class JanrainCAPI {

  private static $capi_url = 'https://v1.api.us.janrain.com/config';
  private $app_id;
  private $client_id;
  private $client_secret;

  public function __construct($settings = array()) {
    $this->app_id = $settings['app_id'];
    $this->client_id = $settings['client_id'];
    $this->client_secret = $settings['client_secret'];
  }

  public function getForms($flow = '') {
    $data = $this->doGet("flows/$flow/forms");
    return json_decode($data);
  }

  public function getFields($flow = '') {
    $data = $this->doGet("flows/$flow/fields");
    return json_decode($data);
  }

  public function getFlowForm($flow = '', $form = '') {
    $data = $this->doGet("flows/$flow/forms/$form");
    return json_decode($data);
  }

  public function getFlowField($flow = '', $field) {
    $data = $this->doGet("flows/$flow/fields/$field");
    return json_decode($data);
  }

  private function doGet($uri) {
    $options['headers'] = array('Authorization' => 'Basic ' . base64_encode($this->client_id . ':' . $this->client_secret));
    $url = self::$capi_url . '/' . $this->app_id . '/' . $uri;

    $result = drupal_http_request($url, $options);

    if ($result->code == 200) {
      return $result->data;
    }
    else {
      watchdog('janrain_user', 'Error in JanrainCAPI->doGet: <br>RESPONSE: @code @error<br>RESPONSE DATA: @data',
        array(
          '@code' => $result->code,
          '@error' => $result->error,
          '@data' => $result->data,
        ),
        WATCHDOG_ERROR
      );
      return FALSE;
    }
  }
}
