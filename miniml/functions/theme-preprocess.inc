<?php

/**
 * Implements hook_preprocess_html()
 */
function miniml_preprocess_html(&$vars) {
  $vars['base_path'] = base_path();
  $vars['path_to_miniml'] = drupal_get_path('theme', 'miniml');

  // Attributes for html element.
  $vars['html_attributes_array'] = array(
    'lang' => $vars['language']->language,
    'dir' => $vars['language']->dir,
  );

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

  // Touch icon for third-generation iPad with high-resolution Retina display.
  drupal_add_html_head(array(
    '#tag' => 'link',
    '#attributes' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '144x144',
      'href' => base_path() . path_to_theme() .'/images/apple-touch-icon-144x144-precomposed.png',
    ),
  ), 'touch-icon-144');

  // Touch icon for iPhone with high-resolution Retina display.
  drupal_add_html_head(array(
    '#tag' => 'link',
    '#attributes' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '114x114',
      'href' => base_path() . path_to_theme() .'/images/apple-touch-icon-114x114-precomposed.png',
    ),
  ), 'touch-icon-114');

  // Touch icon for first- and second-generation iPad.
  drupal_add_html_head(array(
    '#tag' => 'link',
    '#attributes' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '72x72',
      'href' => base_path() . path_to_theme() .'/images/apple-touch-icon-72x72-precomposed.png',
    ),
  ), 'touch-icon-72');

  // Touch icon for non-Retina iPhone, iPod Touch, and Android 2.1+ devices.
  drupal_add_html_head(array(
    '#tag' => 'link',
    '#attributes' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'href' => base_path() . path_to_theme() .'/images/apple-touch-icon-precomposed.png',
    ),
  ), 'touch-icon');

  // Metro tile icon (http://kremalicious.com/add-your-web-site-to-the-windows-8-metro-ui).
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'msapplication-TileImage',
      'content' => base_path() . path_to_theme() .'/images/apple-touch-icon-precomposed.png',
    ),
  ), 'metro-image');

  // Metro tile color.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'msapplication-TileColor',
      'content' => '#ffffff',
    ),
  ), 'metro-color');

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
 * Implements hook_process_html().
 */
function miniml_process_html(&$vars, $hook) {
  // Flatten out html_attributes.
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes_array']);
}

/**
 * Implements hook_preprocess_html_tag().
 */
function miniml_preprocess_html_tag(&$vars) {
  $element = &$vars['element'];

  if (in_array($element['#tag'], array('script', 'link', 'style'))) {
    // Remove the "type" attribute.
    unset($element['#attributes']['type']);

    // Remove redundant type attribute and CDATA comments.
    unset($element['#attributes']['type'], $element['#value_prefix'], $element['#value_suffix']);

    // Remove media="all" but leave others unaffected.
    if (isset($element['#attributes']['media']) && $element['#attributes']['media'] === 'all') {
      unset($element['#attributes']['media']);
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
  if (in_array($block->module, array('menu', 'menu_block'))) {
    $vars['attributes_array']['role'] = 'navigation';
    $vars['theme_hook_suggestions'][] = 'block__nav';
  }

  if ($block->module == 'search') {
    $vars['attributes_array']['role'] = 'search';
  }

  // Test if we even have to have class defined.
  if (!$vars['attributes_array']['class']) {
    unset($vars['attributes_array']['class']);
  }
}

/**
 * Implements hook_process_block().
 */
function miniml_process_block(&$vars, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $vars['title'] = $vars['block']->subject;
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