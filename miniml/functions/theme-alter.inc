<?php

/**
 * Implements hook_html_head_alter()
 */
function miniml_html_head_alter(&$elements) {
  // Remove some meta tag.
  unset($elements['system_meta_generator']);
  unset($elements['metatag_canonical']);
  unset($elements['metatag_generator']);
  unset($elements['metatag_shortlink']);
}

/**
 * Implements hook_page_alter().
 */
function miniml_page_alter(&$page) {
  // Remove all the region wrappers.
  foreach (element_children($page) as $key => $region) {
    if (!empty($page[$region]['#theme_wrappers'])) {
      $page[$region]['#theme_wrappers'] = array_diff($page[$region]['#theme_wrappers'], array('region'));
    }
  }
  // Remove the wrapper from the main content block.
  if (!empty($page['content']['system_main'])) {
    $page['content']['system_main']['#theme_wrappers'] = array_diff($page['content']['system_main']['#theme_wrappers'], array('block'));
  }
}

/**
 * Implements hook_js_alter().
 */
function miniml_js_alter(&$javascript) {
  // Override Drupal's old version of jQuery with our own.
  // http://oldwildissue.wordpress.com/2012/11/19/drupal-7-manually-update-jquery-version
  $version = '1.9.1';
  $path = path_to_theme() .'/javascripts/libs/jquery-'. $version .'.min.js';
  $javascript[$path] = $javascript['misc/jquery.js'];
  $javascript[$path]['version'] = $version;
  $javascript[$path]['data'] = $path;
  unset($javascript['misc/jquery.js']);

  // http://www.metaltoad.com/blog/drupal-7-taking-control-css-and-js-aggregation
  uasort($javascript, 'drupal_sort_css_js');
  $i = 0;
  foreach ($javascript as $name => $script) {
    if ($javascript[$name]['type'] == 'inline' || $javascript[$name]['type'] == 'external') {
      continue;
    }
    $javascript[$name]['weight'] = $i++;
    $javascript[$name]['group'] = JS_DEFAULT;
    $javascript[$name]['every_page'] = FALSE;

    // Move everything but Modernizr to the bottom of the page.
    if (strpos($name, 'modernizr') === FALSE) {
      if ($javascript[$name]['scope'] !== 'footer') {
        $javascript[$name]['scope'] = 'footer';
      }
    }

    // Remove scripts not need by anonymous users.
    if (user_is_anonymous()) {
      if (strpos($name, 'modules') !== FALSE) {
        unset($javascript[$name]);
      }
    }
  }
}

/**
 * Implements hook_css_alter().
 */
function miniml_css_alter(&$css) {
  $keep = array();
  if (user_is_logged_in()) {
    $keep[] = 'admin_menu.css';
    $keep[] = 'admin_menu.uid1.css';
    $keep[] = 'admin_menu_toolbar.css';
    $keep[] = 'contextual.css';
  }

  // http://www.metaltoad.com/blog/drupal-7-taking-control-css-and-js-aggregation
  uasort($css, 'drupal_sort_css_js');
  $i = 0;
  foreach ($css as $name => $style) {
    $css[$name]['weight'] = $i++;
    $css[$name]['group'] = CSS_DEFAULT;
    $css[$name]['every_page'] = FALSE;

    // Only keep theme and a few module stylesheets.
    $file = basename($name);
    if (!in_array($file, $keep) && strpos($name, 'themes') === FALSE) {
      unset($css[$name]);
    }
  }
}