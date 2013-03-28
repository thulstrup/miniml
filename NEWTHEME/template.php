<?php

/**
 * Implements hook_preprocess_html()
 */
function NEWTHEME_preprocess_html(&$vars) {
  drupal_add_css(path_to_theme() .'/stylesheets/style.css', array('group' => CSS_DEFAULT, 'browsers' => array('IE' => 'gte IE 9', '!IE' => TRUE), 'every_page' => TRUE));
  drupal_add_css(path_to_theme() .'/stylesheets/oldie.css', array('group' => CSS_DEFAULT, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'every_page' => TRUE));
}