<div<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>

  <?php if ($block->subject): ?>
    <h2><?php print $block->subject ?></h2>
  <?php endif;?>

  <?php print $content ?>

  <?php print render($title_suffix); ?>
</div>
