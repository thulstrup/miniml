<?php if ($logo): ?>
  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
    <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
  </a>
<?php endif; ?>

<?php print render($page['header']); ?>

<?php print theme('links', array('links' => $main_menu, 'heading' => t('Main menu'))); ?>

<?php print $breadcrumb; ?>

<?php print $messages; ?>

<?php print render($page['highlighted']); ?>

<?php if ($title): ?>
  <?php print render($title_prefix); ?>
  <h1><?php print $title; ?></h1>
  <?php print render($title_suffix); ?>
<?php endif; ?>

<?php print render($tabs); ?>

<?php print render($page['help']); ?>

<?php if ($action_links): ?>
  <ul class="action-links"><?php print render($action_links); ?></ul>
<?php endif; ?>

<?php print render($page['content']); ?>

<?php print render($page['sidebar_first']); ?>

<?php print render($page['sidebar_second']); ?>

<?php print render($page['footer']); ?>
