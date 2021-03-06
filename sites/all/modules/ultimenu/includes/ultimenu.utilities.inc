<?php

/**
 * @file
 * Misc functions that hardly change.
 */

/**
 * Returns the array of enabled Ultimenu regions based on enabled settings.
 */
function ultimenu_regions_enabled($delta = NULL) {
  $regions = &drupal_static(__FUNCTION__);

  if (!isset($regions)) {
    $regions     = array();
    $regions_all = ultimenu_regions($delta);

    // First limit to enabled regions from the settings.
    if (($regions_enabled = ultimenu_get_settings('regions')) !== NULL) {
      foreach (array_filter($regions_enabled) as $enabled) {
        // We must depends on enabled menu items as always.
        // A disabled menu item will automatically drop its Ultimenu region.
        if (array_key_exists($enabled, $regions_all)) {
          $regions[$enabled] = $regions_all[$enabled];
        }
      }
    }
  }

  return $regions;
}

/**
 * The array of available Ultimenu regions based on enabled menu items.
 *
 * @return array
 *   An array of regions definition dependent on available main-menu items.
 *
 * @see transliteration_clean_filename()
 * @see menu_navigation_links()
 */
function ultimenu_regions($delta = NULL) {
  $regions = &drupal_static(__FUNCTION__);
  if (!isset($regions)) {
    $blocks      = ultimenu_get_settings('blocks');
    $goodies     = ultimenu_get_settings('goodies');
    $is_mlid     = !empty($goodies['ultimenu-mlid']) ? TRUE : FALSE;
    $menu_blocks = is_array($blocks) ? array_filter($blocks) : array($blocks);

    $menus = array();

    if ($delta) {
      $menus[$delta] = menu_navigation_links($delta);
    }
    else {
      foreach ($menu_blocks as $menu_name => $title) {
        $menus[$menu_name] = menu_navigation_links($menu_name);
      }
    }

    // Allow alteration.
    drupal_alter('ultimenu_menus_info', $menus);

    $regions = array();
    foreach ($menus as $menu_key => $menu_items) {
      foreach ($menu_items as $item_key => $item) {
        $name_id      = ultimenu_truncate_menu_property($menu_key);
        $name_id_nice = str_replace("_", " ", $name_id);
        $menu_title   = check_plain($item['title']);
        if ($is_mlid) {
          $item_id = str_replace('menu-', '', $item_key);
        }
        else {
          $item_id = ultimenu_truncate_menu_property($menu_title, 28);
        }

        $regions["ultimenu_{$name_id}_{$item_id}"] = "Ultimenu:$name_id_nice: $menu_title";
        $regions = array_unique($regions);
      }
    }
    // Allow alteration.
    drupal_alter('ultimenu_regions_info', $regions);
  }

  return $regions;
}

/**
 * Gets Ultimenu regions from the default theme .info.
 */
function ultimenu_get_ultimenu_theme_info() {
  $regions = &drupal_static(__FUNCTION__);

  if (!isset($regions)) {
    $theme = variable_get('theme_default', 'bartik');
    $file = drupal_get_path('theme', $theme) . '/' . $theme . '.info';

    // Parse theme .info file.
    $info = drupal_parse_info_file($file);

    $regions = array();
    foreach ($info['regions'] as $key => $region) {
      if (array_key_exists($key, ultimenu_regions())) {
        $regions[$key] = $region;
      }
    }
  }

  return $regions;
}

/**
 * Colllects unwanted Ultimenu regions for removal stored in the theme .info.
 *
 * When a menu item disabled or deleted, relevant dynamic Ultimenu regions
 * removed from the system, but theme .info has a copy stored there.
 * System will always keep and display the unwanted.
 * Either manually delete it from .info, or do a force removal if so configured.
 *
 * @return array|bool
 *   The array of unwanted Ultimenu regions from theme .info, or FALSE.
 */
function ultimenu_remove_ultimenu_theme_info() {
  $goodies = ultimenu_get_settings('goodies');
  if (!empty($goodies['force-remove-region']) && $theme_regions = ultimenu_get_ultimenu_theme_info()) {
    return $theme_regions;
  }

  return FALSE;
}

/**
 * Simplify menu names or menu item titles for region key.
 *
 * If region key is to use menu item title:
 * Region key: ultimenu_LOOOOOOOOOOOOOONGMENUNAME_LOOOOOOOOOOOOOOOOOONGMENUITEM.
 * If region key is to use unfriendly key MLID, we'll only care for menu name.
 * Region key: ultimenu_LOOOOOOOOOOOOOONGMENUNAME_1234
 *
 * @param string $property
 *   The Menu name or menu item title
 *
 * @param int $max_length
 *   The amount of characters to truncate.
 *
 * @return string
 *   The truncated menu properties.
 */
function ultimenu_truncate_menu_property($property, $max_length = 60) {
  // @todo: its very expensive, add option.
  // if (function_exists('transliteration_clean_filename')) {
  // $property = transliteration_clean_filename($property);
  // }
  $property = strtolower(str_replace(array('menu-', '-menu'), '', $property));
  $property = str_replace(array('-', " ", "'"), '_', $property);
  $property = truncate_utf8($property, $max_length, TRUE, FALSE);
  return $property;
}

/**
 * Get all available menus, excluding some admin menus.
 */
function ultimenu_get_menus() {
  $menus = &drupal_static(__FUNCTION__);
  if (!isset($menus)) {
    $available_menus = menu_get_menus();
    $exclude_menus = array(
      'devel' => t('Development'),
      'management' => t('Management'),
      'navigation' => t('Navigation'),
    );
    $menus = array_diff_key($available_menus, $exclude_menus);
  }

  return $menus;
}

/**
 * Collects enabled Ultimenu block items.
 */
function ultimenu_get_blocks() {
  $items = &drupal_static(__FUNCTION__);
  if (!isset($items)) {
    $items = array();
    $menus = ultimenu_get_menus();
    foreach ($menus as $delta => $name) {
      if (ultimenu_blocks_enabled($delta)) {
        $items[$delta] = t("@name", array('@name' => $name));
      }
    }
  }

  return $items;
}

/**
 * Check for an Ultimenu block enabled by settings.
 */
function ultimenu_blocks_enabled($delta) {
  $blocks = ultimenu_get_settings('blocks');
  if (!empty($blocks[$delta])) {
    return TRUE;
  }

  return FALSE;
}

/**
 * A wrapper function for variable_get.
 *
 * Allows storing variables in one place rather than setting and calling each.
 */
function ultimenu_get_settings($setting_name = NULL, $default = NULL) {
  $cache = &drupal_static(__FUNCTION__);

  if (empty($cache)) {
    $settings = variable_get('ultimenu_settings', array());
    if (isset($settings[$setting_name]) && ($setting = $settings[$setting_name]) !== NULL) {
      return $setting;
    }
  }
  else {
    if (isset($cache[$setting_name])) {
      return $cache[$setting_name];
    }
  }

  return $default;
}

/**
 * The configuration for the requested block delta.
 *
 * @param string $delta
 *   The delta that uniquely identifies the block in the block system. If
 *   not specified, the default configuration will be returned.
 *
 * @param bool $original
 *   If TRUE, return as is, otherwise convert URI to actual path.
 *
 * @return array
 *   An associated array of configuration options.
 */
function ultimenu_get_config($delta = NULL, $original = FALSE) {
  $safe_delta = ultimenu_truncate_menu_property($delta);
  $config = array(
    'skin' => '',
    'skin_name' => '',
  );

  // Get the block configuration options.
  if ($delta) {
    static $blocks, $regions;
    if (!isset($blocks)) {
      $blocks = ultimenu_get_settings('blocks');
    }
    if (!empty($blocks[$delta])) {
      $config['delta'] = $delta;
    }

    if (!isset($regions)) {
      $regions = ultimenu_get_settings('regions');
    }

    // Structure: main_menu => ultimenu-htb:module|ultimenu-orange.css.
    $menu_name = str_replace(array("-", "'"), "_", $delta);
    $config['menu_name'] = $delta;
    $config['menu_name_truncated'] = ultimenu_truncate_menu_property($delta);

    $setting = ultimenu_get_settings($menu_name);
    list($config['orientation'], $uri) = array_pad(array_map('trim', explode(":", $setting, 2)), 2, NULL);

    if ($original) {
      $config['skin'] = $uri;
    }
    else {
      $theme_skin  = drupal_get_path('theme', variable_get('theme_default', 'bartik')) . '/css/ultimenu';
      $custom_skin = ultimenu_get_settings('skins');
      $module_skin = drupal_get_path('module', 'ultimenu') . '/skins';

      if (strpos($uri, "module|") !== FALSE) {
        $config['skin'] = str_replace("module|", $module_skin . '/', $uri);
      }
      elseif (strpos($uri, "custom|") !== FALSE) {
        $config['skin'] = str_replace("custom|", $custom_skin . '/', $uri);
      }
      elseif (strpos($uri, "theme|") !== FALSE) {
        $config['skin'] = str_replace("theme|", $theme_skin . '/', $uri);
      }
    }
    if ($skin_name = $uri) {
      $skin_name = str_replace(".css", '', substr($skin_name, strrpos($skin_name, '|') + 1));
      $config['skin_name'] = drupal_html_class($skin_name);
    }
  }

  return $config;
}

/**
 * A helper function to generate a list of blocks from a specified region.
 *
 * @param array $region
 *   The string identifier for a Ultimenu region. e.g. "ultimenu_main_about"
 *
 * @return array
 *   The renderable array of blocks within the region.
 */
function ultimenu_block_get_blocks_by_region($region) {
  $build = array();
  if ($blocks = block_get_blocks_by_region($region)) {
    $build = $blocks;
  }
  if (function_exists('context_get_plugin') && $context = context_get_plugin('reaction', 'block')) {
    if ($blocks_context = $context->block_get_blocks_by_region($region)) {
      $build = array_merge($blocks_context, $build);
    }
  }

  return $build;
}
