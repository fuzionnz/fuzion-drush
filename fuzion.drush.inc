<?php

/**
 * @file
 * Provide Drush commands for Fuzion.
 */

/**
 * Validate whether pm-download is permitted.
 */
function drush_fuzion_pm_download_pre_validate() {
  return _fuzion_validate_clean_git();
}

/**
 * Validate whether pm-enable is permitted.
 */
function drush_fuzion_pm_enable_pre_validate() {
  return _fuzion_validate_clean_git();
}

/**
 * Check if a site's codebase is clean.
 *
 * @see https://redmine.fuzion.co.nz/issues/7205
 *
 * Just an initial test to see ... but IMO this is likely to get in
 * the way more than it is to help, since if it interrupts builds, it
 * also interferes with tools like `drush make`. So I'm going to stash
 * it for now.
 *
 * TBH I'd rather not even have warnings come out of builds. (We could
 * add checks for "is this drush make calling drush pm-enable" but
 * that seems realy hard at 10:30 on Friday!)
 */
function _fuzion_validate_clean_git() {
  $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  if (file_exists($path . '/.git')) {
    drush_log('Checking git status before modifying codebase.', 'ok');
    $result = drush_shell_cd_and_exec($path, 'git status --porcelain');
    if ($output = drush_shell_exec_output()) {
      $error_message = '`git status` does not look clean. Please commit changes before modifying code further.';
      drush_log($error_message, 'warning');
      drush_log(implode("\n", $output), 'warning');
    }
  }
  else {
    drush_log(dt('There is no Git repository initialized in @path', array('@path' => $path)), 'warning');
  }
}

/**
 * Implements hook_drush_command().
 */
function fuzion_drush_command() {
  $items['fz-install-archimedes'] = array(
    'description' => 'Install Archimedes on a site.',
    'aliases' => array('arch-install'),
  );
  $items['fz-new-site'] = array(
    'description' => 'Install a new site with Fuzion Drupal + Fuzion CiviCRM.',
    'aliases' => array('fz-newsite'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'destination' => 'The directory to create the new site in.',
    ),
  );
  return $items;
}

/**
 * Work around lack of Archimedes package on d.o and install it via
 * makefile.
 */
function drush_fuzion_fz_install_archimedes() {
  $version_parts = explode('.', VERSION);
  $version_major = reset($version_parts);
  $makefile = dirname(__FILE__) . '/makefiles/archimedes-' . $version_major . '.make';
  $verbose = drush_get_context('DRUSH_VERBOSE') ? '-v' : '';
  // Install Archimedes for the appropriate version.
  $command = 'drush ' . $verbose . ' -y make --no-core ' . $makefile;
  drush_log($command, 'ok');
  if (!drush_shell_exec($command)) {
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_log('Output of ' . $command . ':', 'error');
      drush_log(implode("\n", drush_shell_exec_output()), 'warning');
    }
    return drush_set_error('DRUSH_FUZION_ARCHIMEDES_INSTALL_PROBLEM', dt('Unable to install Archimedes via !command.', array('!command' => $command)));
  }
  // Enable Archimedes and Archimedes Extras
  $command = 'drush ' . $verbose . ' -y en archimedes archimedes_extras';
  drush_log($command, 'ok');
  if (!drush_shell_exec($command)) {
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_log('Output of ' . $command . ':', 'error');
      drush_log(implode("\n", drush_shell_exec_output()), 'warning');
    }
    return drush_set_error('DRUSH_FUZION_ARCHIMEDES_ENABLE_PROBLEM', dt('Unable to enable Archimedes via !command.', array('!command' => $command)));
  }
}

/**
 * Install Fuzion Drupal 7 profile + Fuzion CiviCRM.
 */
function drush_fuzion_fz_new_site() {
  // Path is required.
  // return drush_set_error('DRUSH_FUZION_MISSING_DESTINATION', dt('
  $makefile = dirname(__FILE__) . '/makefiles/fuzion-drupal7.make';
  $command = 'drush -v -y make ' . $makefile;
  if (!drush_shell_exec($command)) {
    return drush_set_error('DRUSH_FUZION_PROFILE_INSTALL_PROBLEM', dt('Unable to install Fuzion Drupal profile via !command.', array('!command' => $command)));
  }
}
