<div<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>

  <?php if ($block->title): ?>
    <h2><?php print $block->title ?></h2>
  <?php endif;?>

  <?php print $content ?>

  <?php print render($title_suffix); ?>
</div>
