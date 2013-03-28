<?php

/**
 * Prepares a string for use as a valid class name.
 */
function miniml_clean_string($str) {
  $transliterate = module_exists('transliteration');
  $pathauto = module_exists('pathauto');

  if ($pathauto) {
    module_load_include('inc', 'pathauto');
    $str = pathauto_cleanstring($str);
  }

  if ($transliterate) {
    $str = transliteration_clean_filename($str);
  }

  if (!$pathauto && !$transliterate) {
    $str = drupal_html_class($str);
  }

  return $str;
}