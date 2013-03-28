<?php

/**
 * Implements hook_preprocess_html()
 */
function miniml_preprocess_html(&$vars) {
  drupal_add_js(array('themePath' => path_to_theme()), 'setting');

  // Viewport.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  ), 'viewport');

  // Force IE to use most up-to-date render engine.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible',
      'content' => 'IE=edge,chrome=1',
    ),
  ), 'x-ua-compatible');

  // iOS friendly page title.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'apple-mobile-web-app-title',
      'content' => variable_get('site_name', 'miniml'),
    ),
  ), 'apple-mobile-web-app-title');

  // Windows Phone 8 Viewport Fix: http://trentwalton.com/2013/01/16/windows-phone-8-viewport-fix/
  drupal_add_js("if (navigator.userAgent.match(/IEMobile\/10\.0/)) {
      var msViewportStyle = document.createElement('style');
      msViewportStyle.appendChild(document.createTextNode('@-ms-viewport{width:auto!important}'));
      document.getElementsByTagName('head')[0].appendChild(msViewportStyle);
    }", array(
    'type' => 'inline',
    'scope' => 'header',
    'group' => JS_THEME,
    'every_page' => TRUE,
    'weight' => -1,
  ));

  // Body attributes.
  $remove_classes = array('html', 'front', 'not-front', 'logged-in', 'not-logged-in', 'no-sidebars', 'one-sidebar sidebar-first', 'one-sidebar sidebar-second', 'two-sidebars');
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], $remove_classes));
  $vars['classes_array'] = preg_grep('/^page-node/', $vars['classes_array'], PREG_GREP_INVERT);
  $vars['classes_array'] = preg_grep('/^page-user/', $vars['classes_array'], PREG_GREP_INVERT);
  $vars['classes_array'] = preg_grep('/^node-type/', $vars['classes_array'], PREG_GREP_INVERT);

  // Add classes to attributes array.
  $vars['attributes_array']['class'] = $vars['classes_array'];

  // Test if we even have to have class defined.
  if (!$vars['attributes_array']['class']) {
    unset($vars['attributes_array']['class']);
  }
}

/**
 * Implements hook_preprocess_html_tag().
 */
function miniml_preprocess_html_tag(&$vars) {
  $element = &$vars['element'];

  if ($element['#tag'] == 'meta' && isset($element['#attributes']['http-equiv']) && $element['#attributes']['http-equiv'] == 'Content-Type') {
    $element['#attributes'] = array(
     'charset' => 'utf-8',
    );
  }

  if (in_array($element['#tag'], array('script', 'link', 'style'))) {
    // Remove the "type" attribute.
    unset($element['#attributes']['type']);

    // Remove CDATA comments.
    if (isset($element['#value_prefix']) && ($element['#value_prefix'] == "\n<!--//--><![CDATA[//><!--\n" || $element['#value_prefix'] == "\n<!--/*--><![CDATA[/*><!--*/\n")) {
      unset($element['#value_prefix']);
    }
    if (isset($element['#value_suffix']) && ($element['#value_suffix'] == "\n//--><!]]>\n" || $element['#value_suffix'] == "\n/*]]>*/-->\n")) {
      unset($element['#value_suffix']);
    }
  }
}

/**
 * Implements hook_preprocess_page().
 */
function miniml_preprocess_page(&$vars) {
}

/**
 * Implements hook_preprocess_node().
 */
function miniml_preprocess_node(&$vars) {
  $node = $vars['node'];

  // Modify classes.
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('node', 'node-sticky','node-unpublished', 'node-promoted')));
  $vars['classes_array'] = preg_grep('/^node-/', $vars['classes_array'], PREG_GREP_INVERT);
  $vars['attributes_array']['class'] = $vars['classes_array'];
  $vars['attributes_array']['role'] = 'article';

  // Test if we even have to have class defined.
  if (!$vars['attributes_array']['class']) {
    unset($vars['attributes_array']['class']);
  }
}

/**
 * Implements hook_preprocess_block().
 */
function miniml_preprocess_block(&$vars) {
  $block = $vars['block'];

  // Modify classes.
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('block')));
  $vars['attributes_array']['class'] = preg_grep('/^block-/', $vars['classes_array'], PREG_GREP_INVERT);

  // Template for navigation blocks.
  if (in_array($block->delta, array('main-menu', 'navigation', 'management'))) {
    $vars['attributes_array']['role'] = 'navigation';
    $vars['theme_hook_suggestions'][] = 'block__nav';
  }

  // Test if we even have to have class defined.
  if (!$vars['attributes_array']['class']) {
    unset($vars['attributes_array']['class']);
  }
}

/**
 * Implements template_preprocess_field().
 */
function miniml_preprocess_field(&$vars) {
  // If the view mode is "full" use <h2> for the field labels. Otherwise,
  // assume a teaser or more compact view mode is being displayed, and use <h3>.
  $vars['heading'] = ($vars['element']['#view_mode'] == 'full') ? 'h2' : 'h3';

  // Add a less verbose field name class: .field-NAME
  $vars['classes_array'][] = drupal_html_class($vars['element']['#field_name']);

  // Use .field-body instead of .body
  if ($vars['element']['#field_name'] == 'body') {
    $vars['classes_array'][] = 'field-body';
  }

  // Remove some classes from the wrapper <div>.
  $classes_to_remove = array(
    'field',
    'field-label-'. drupal_html_class($vars['element']['#label_display']),
    'field-type-'. drupal_html_class($vars['element']['#field_type']),
    'field-name-'. drupal_html_class($vars['element']['#field_name']),
    'body',
    'clearfix',
  );
  $vars['classes_array'] = array_diff($vars['classes_array'], $classes_to_remove);
}

/**
 * Implements template_preprocess_views_view().
 */
function miniml_preprocess_views_view(&$vars) {
  //$vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('view')));
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('view-'. $vars['name'])));
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('view-id-'. $vars['name'])));
  $vars['classes_array'] = array_values(array_diff($vars['classes_array'], array('view-display-id-'. $vars['display_id'])));
  $vars['classes_array'] = preg_grep('/^view-dom-id-/', $vars['classes_array'], PREG_GREP_INVERT);
}

/**
 * Implements of template_preprocess_views_view_list().
 */
function miniml_preprocess_views_view_list(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $vars['classes_array'] = array();
  $vars['classes'] = array();

  foreach ($rows as $id => $row) {
    //$vars['classes'][$id][] = 'views-row';

    if ($row_class = $view->style_plugin->get_row_class($id)) {
      $vars['classes'][$id][] = $row_class;
    }

    if ($vars['classes']  && $vars['classes'][$id]) {
      $vars['classes_array'][$id] = implode(' ', $vars['classes'][$id]);
    }
    else {
      $vars['classes_array'][$id] = '';
    }
  }
}

/**
 * Display the simple view of rows one after another
 */
function miniml_preprocess_views_view_unformatted(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $style = $view->style_plugin;
  $options = $style->options;

  $vars['classes_array'] = array();
  $vars['classes'] = array();
  $default_row_class = isset($options['default_row_class']) ? $options['default_row_class'] : FALSE;
  $row_class_special = isset($options['row_class_special']) ? $options['row_class_special'] : FALSE;
  // Set up striping values.
  $count = 0;
  $max = count($rows);
  foreach ($rows as $id => $row) {
    $count++;
    if ($default_row_class) {
      //$vars['classes'][$id][] = 'views-row';
      //$vars['classes'][$id][] = 'views-row-' . $count;
    }
    if ($row_class_special) {
      //$vars['classes'][$id][] = 'views-row-' . ($count % 2 ? 'odd' : 'even');
      if ($count == 1) {
        //$vars['classes'][$id][] = 'views-row-first';
      }
      if ($count == $max) {
        //$vars['classes'][$id][] = 'views-row-last';
      }
    }

    if ($row_class = $view->style_plugin->get_row_class($id)) {
      $vars['classes'][$id][] = $row_class;
    }

    // Flatten the classes to a string for each row for the template file.
    $vars['classes_array'][$id] = isset($vars['classes'][$id]) ? implode(' ', $vars['classes'][$id]) : '';
  }
}