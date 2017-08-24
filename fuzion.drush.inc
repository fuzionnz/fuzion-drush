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
    $result = drush_shell_cd_and_exec($path, 'git status --porcelain');
    if ($output = drush_shell_exec_output()) {
      $messages[] = '`git status` does not look clean. Please commit changes before modifying code further.';
      $messages += $output;
      return $messages;
    }
  }
  else {
    return array(dt('There is no Git repository initialized in @path', array('@path' => $path)));
  }
}

/**
 * Warn if Git not in clean state.
 */
function _fuzion_warn_clean_git() {
  if ($messages = _fuzion_validate_clean_git()) {
    drush_log(array_shift($messages), 'warning');
    if (!empty($messages)) {
      drush_log(implode("\n", $messages), 'warning');
    }
    return TRUE;
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
  $items['fz-install-civicrm'] = array(
    'description' => 'Install / upgrade Archimedes on a site.',
    'aliases' => array('fz-upgrade-civicrm'),
    'arguments' => array(
      'release' => 'Name of Fuzion CiviCRM release to install, eg "4.7.22.1".',
    ),
    'options' => array(
    ),
  );
  $items['fz-new-site'] = array(
    'description' => 'Install a new site with Fuzion Drupal + Fuzion CiviCRM.',
    'aliases' => array('fz-newsite'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'destination' => 'The directory to create the new site in.',
    ),
  );
  $items['fz-civicrm-upgrade'] = array(
    'description' => "Replace CiviCRM codebase with new specified tarfile and upgrade database",
    'examples' => array(
      'drush fz-civicrm-upgrade' =>
      'Replace old CiviCRM codebase with new codebase and run upgrade process.',
    ),
    'options' => array(
      'dist-url' => 'Base URL of CiviCRM dist server, eg http://dist.civicrm.org',
      'commit' => 'Commit message to apply post-upgrade.',
      'min-version' => 'Required version to commence upgrade, eg --version-min=4.5 to avoid upgrading sites still on 4.4',
      'skip-backups' => 'Do not make DB backups pre-upgrade.',
      'skip-git' => 'Ignore missing or unclean Git repository.',
      'aliases' => array('fz-cvup'),
      'tarball-release' => 'CiviCRM release (eg "4.6.9") to install. Defaults to latest stable release.',
      'tarball-tag' => 'Release tag (eg "release1"). Defaults empty, required for Fuzion tarballs where release and tag differ.',
      'tarball-date' => 'Date (eg 20151012) or "nightly". Required for Fuzion tarballs.',
    ),
  );
  return $items;
}

/**
 * Work around lack of Archimedes package on d.o and install it via makefile.
 */
function drush_fuzion_fz_install_civicrm($release = '') {
  // @TODO Warn if called with fz-civicrm-upgrade syntax?

  // @TODO Require a release (that exists)?

  // Get Drupal version - support multiple versions by different .make files.
  $version_parts = explode('.', VERSION);
  $version_major = reset($version_parts);

  // Link to URL ideally. Check we can grab it with file_get_contents()?
  $makefile = dirname(__FILE__) . '/makefiles/fuzion-civicrm-' . $release . '-drupal' . $version_major . '.make';
  $makeurl = "https://raw.githubusercontent.com/fuzionnz/fuzion-drush/master/makefiles/fuzion-civicrm-{$release}-drupal{$version}.make";

  if (!$make = file_get_contents) {
    return drush_set_error('DRUSH_FUZION_CIVICRM_NO_MAKEFILE_FOUND', dt('Unable to get makefile: @makeurl', ['@makeurl' => $makeurl]));
  }

  $verbose = drush_get_context('DRUSH_VERBOSE') ? '-v' : '';
  // Install CiviCRM using the appropriate make URL.
  $command = 'drush ' . $verbose . ' -y make --no-core ' . $makeurl;
  drush_log($command, 'ok');
  if (!drush_shell_exec($command)) {
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_log('Output of ' . $command . ':', 'error');
      drush_log(implode("\n", drush_shell_exec_output()), 'warning');
    }
    return drush_set_error('DRUSH_FUZION_CIVICRM_INSTALL_PROBLEM', dt('Unable to install Fuzion CiviCRM via !command.', array('!command' => $command)));
  }
}

/**
 * Work around lack of Archimedes package on d.o and install it via makefile.
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
  // Enable Archimedes and Archimedes Extras.
  $command = 'drush ' . $verbose . ' -y en archimedes archimedes_extras';
  // If CiviCRM is enabled, add Archimedes CiviCRM.
  if (module_exists('civicrm')) {
    $command .= ' archimedes_civicrm';
  }
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

/**
 * Upgrade CiviCRM.
 */
function drush_fuzion_fz_civicrm_upgrade() {
  $root = drush_get_context('DRUSH_DRUPAL_CORE');
  $uri = drush_get_context('DRUSH_URI');
  $drush_cmd = 'drush -r ' . $root . ' -l ' . $uri;

  $yes = drush_get_option('yes');
  $verbose = drush_get_context('DRUSH_VERBOSE');

  if (!drush_get_option('skip-git') && _fuzion_warn_clean_git()) {
    return drush_set_error('DRUSH_FUZION_GIT_UNCOMMITTED', dt('Drupal site has missing or unclean Git repo.'));
  }

  // Check that the site already has a minimum version specified.
  if (!$min_version = drush_get_option('min-version')) {
    $min_version = '0';
  }
  try {
    if (!function_exists('civicrm_initialize')) {
      return drush_set_error('DRUSH_FUZION_NO_CIVICRM_INITIALIZE', dt('Unable to find function civicrm_initialize, does this site really have CiviCRM installed?'));
    }
    civicrm_initialize();
    $code_version = CRM_Utils_System::version();
    $db_version = CRM_Core_BAO_Domain::version();
    if ($code_version != $db_version) {
      return drush_set_error('DRUSH_FUZION_CIVICRM_VERSIONMISMATCH', dt('CiviCRM DB and code versions differ. Please resolve this then try again.'));
    }
    if (!version_compare($code_version, $min_version, '>=')) {
      return drush_set_error('DRUSH_FUZION_CIVICRM_VERSIONINSUFFICENT', dt('CiviCRM version @codever is less than specified minimum version @minver.', array('@codever' => $code_version, '@minver' => $min_version)));
    }
  }
  catch (Exception $e) {
    return drush_set_error('DRUSH_FUZION_CIVICRM_NOTINITIALIZED', dt('CiviCRM did not initialize.'));
  }

  // Retrieve the latest stable release from CiviCRM.org, unless we've been
  // instructed to use a specific release via --release=X.X.X
  if (!$release = drush_get_option('tarball-release')) {
    $version_url = 'http://latest.civicrm.org/versions.json';
    $req = drupal_http_request($version_url);
    if ($req->error) {
      return drush_set_error('DRUSH_FUZION_ERR_FETCHING_CIVICRM_VERSION', dt('Unable to retrieve CiviCRM version data from @version_url', array('@version_url' => $version_url)));
    }
    $release = _drush_fuzion_find_latest_stable(json_decode($req->data));
  }

  // Parameter for Fuzion's special tags. (Fuzion dist server has -release1 in
  // the directory, need to check with Eileen why this is.)
  if (!$tag = drush_get_option('tarball-tag')) {
    // CiviCRM dist server uses major.minor branch.
    $data = explode('.', $release);
    $tag = $data[0] . '.' . $data[1];
  }

  // Parameter for build date. (Fuzion uses "nightly" only, but CiviCRM.org has
  // dates, and this may be a fann_get_sarprop_step_error_threshold_factor if
  // you're ahead of US time.)
  if (!$tarball_date = drush_get_option('tarball-date')) {
    // This doesn't account for the CiviCRM server being in US time.
    // Tarballs seem to arrive between 10am and 2pm NZDST?
    $tarball_date = date('Ymd');
  }

  // Drupal version.
  if (!defined('VERSION')) {
    return drush_set_error('DRUSH_FUZION_NO_DRUPAL_VERSION_DEFINED', dt('Drupal version is not defined.'));
  }

  // Drupal 6
  if (version_compare(VERSION, 6, '>=') && !version_compare(VERSION, 7, '>=')) {
    $drupal = 'drupal6';
  }
  // Drupal 7
  if (version_compare(VERSION, 7, '>=') && !version_compare(VERSION, 8, '>=')) {
    $drupal = 'drupal';
  }
  // Drupal 8
  if (version_compare(VERSION, 8, '>=') && !version_compare(VERSION, 9, '>=')) {
    $drupal = 'drupal8';
  }

  if (!$dist_url = drush_get_option('dist-url')) {
    $dist_url = 'http://dist.civicrm.org';
    // http://dist.fudev.co.nz/by-date/latest/4.6.9-release1/civicrm-4.6.9-drupal-nightly.tar.gz
    // http://dist.civicrm.org/by-date/latest/4.6/civicrm-4.6.10-drupal-20151012.tar.gz
  }

  $params = array(
    '@tag' => $tag,
    '@release' => $release,
    '@drupal' => $drupal,
    '@url' => $dist_url,
    '@date' => $tarball_date,
  );
  $tarball_url = dt('@url/by-date/latest/@tag/civicrm-@release-@drupal-@date.tar.gz', $params);

  $cache_duration = (drush_get_option('cache', TRUE)) ? 86400 * 365 : 0;
  $filename = str_replace('?', '_', basename($tarball_url));
  $tmp_path = drush_tempdir() . '/' . $filename;
  $download_path = drush_download_file($tarball_url, $tmp_path, $cache_duration);

  if ($download_path || drush_get_context('DRUSH_SIMULATE')) {
    if ($verbose) {
      drush_log(dt('Downloading !filename was successful.', array('!filename' => $filename)));
    }
  }
  else {
    $args = array(
      '!version' => 'CiviCRM ' . $release,
      '!path' => $tmp_path,
      '!url' => $tarball_url,
    );
    $msg = dt('Unable to download !version to !path from !url.', $args);
    return drush_set_error('DRUSH_FUZION_DOWNLOAD_FAILED', $msg);
  }

  // Get a commit message to log with this upgrade.
  if (!$commitmsg = drush_get_option('commit')) {
    $commitmsg = dt('CiviCRM upgrade to @version', array('@version' => $release));
    if (!$yes) {
      $commitmsg = drush_prompt(dt('Commit message'), $commitmsg);
    }
  }

  if (!drush_get_option('skip-backups')) {
    if (!$result_file = drush_get_option('result-file')) {
      $sql_conf = drush_sql_conf();
      $result_file = getenv('HOME') . '/' . $sql_conf['database'] . '-' . date('Ymdhms') . '.sql';
    }
    $command = dt('@drush civicrm-sql-dump --result-file=@result_file', array('@drush' => $drush_cmd, '@result_file' => $result_file));
    if (drush_shell_exec($command)) {
      if ($verbose) {
        drush_log(dt('Backed up CiviCRM DB in @result_file', array('@result_file' => $result_file)), 'ok');
      }
    }
    else {
      return drush_set_error('DRUSH_FUZION_SQLDUMP_FAILED', dt('Unable to make snapshot of DB to @result_file', array('@result_file' => $result_file)));
    }
  }

  // Detect which flavour of tar is installed: GNU or BSD.
  $command = 'tar --version | grep -iq gnu';
  if (drush_shell_exec($command)) {
    $tar_args = '--no-overwrite-dir';
  }
  else {
    $tar_args = '';
  }

  // OK, we're ready to go. We have a tarball, a commit message.
  $params = array(
    '@filename' => $download_path,
    '@tar_args' => $tar_args,
  );

  $command = dt('tar -xf @filename @tar_args -C sites/all/modules', $params);
  if (!drush_shell_exec($command)) {
    $message[] = dt('Unable to extract tarball at @filename', array('@filename' => $download_path));
    $message[] = dt('Command was: @command', array('@command' => $command));
    $message += drush_shell_exec_output();
    return drush_set_error('DRUSH_FUZION_CIVICRM_EXTRACT_FAILED', implode(PHP_EOL, $message));
  }

  // Ensure some files do not exist.
  $files_to_remove = array('sites/all/modules/civicrm/.gitignore');
  foreach ($files_to_remove as $filename) {
    $command = 'rm %s';
    if (drush_shell_exec($command, $filename)) {
      drush_log(dt('Removed @filename from filesystem', array('@filename' => $filename)));
    }
    $command = 'git rm %s';
    if (drush_shell_exec($command, $filename)) {
      drush_log(dt('Removed @filename from git', array('@filename' => $filename)));
    }
  }

  // Ensure that sites/all/modules/civicrm is not a submodule.
  $submodules_to_gitrm = array(
    'sites/all/modules/civicrm',
    'sites/all/modules/civicrm/drupal',
    'sites/all/modules/civicrm/packages',
  );
  foreach ($submodules_to_gitrm as $path) {
    if (drush_shell_exec('git rm --cached %s', $path)) {
      drush_log(dt('Removed submodule @path', array('@path' => $path)), 'ok');
    }
  }

  // Ensure that civicrm.settings.php is excluded by sites/.gitignore -
  // Using sites/.gitignore ensures that Drupal upgrade doesn't overwrite the
  // change on upgrade, and that the .gitignore affects sites/example.org also.
  if (!drush_shell_exec('grep civicrm.settings.php sites/.gitignore')) {
    $command = "echo '*/civicrm.settings.php' >> sites/.gitignore";
    if (drush_shell_exec($command)) {
      if ($verbose) {
        drush_log('Added civicrm.settings.php to sites/.gitignore', 'ok');
      }
      drush_shell_exec('git add sites/.gitignore');
      drush_shell_exec('git commit -m "Ignore civicrm.settings.php"');
    }
  }

  // Add updated CiviCRM files to git.
  if (drush_shell_exec('git add -A sites/all/modules/civicrm/* sites/all/modules/civicrm/.*')) {
    if ($verbose) {
      drush_log('Staged files in sites/all/modules/civicrm/* for commit.', 'ok');
    }
  }

  // Commit changes, if required.
  $command = 'git diff-index --cached --quiet HEAD';
  if (drush_shell_exec($command)) {
    drush_log('No changes to commit.', 'ok');
  }
  else {
    $command = 'git commit -m ' . escapeshellarg($commitmsg);
    if (drush_shell_exec($command)) {
      if ($verbose) {
        drush_log('Committed changes.', 'ok');
      }
    }
    else {
      $message[] = dt('Unable to commit changes to git.', array('@filename' => $download_path));
      $message[] = dt('Command was: @command', array('@command' => $command));
      $message += drush_shell_exec_output();
      return drush_set_error('DRUSH_FUZION_CIVICRM_EXTRACT_FAILED', implode(PHP_EOL, $message));
    }
  }

  if (drush_shell_exec(dt('@drush civicrm-upgrade-db', array('@drush' => $drush_cmd)))) {
    if ($verbose) {
      drush_log('Upgraded CiviCRM DB.', 'ok');
    }
  }
  else {
    $message[] = dt('Unable to upgrade CiviCRM DB.');
    $message += drush_shell_exec_output();
    return drush_set_error('DRUSH_FUZION_CIVICRM_CACHEFLUSH_FAILED', implode(PHP_EOL, $message));
  }

  if (drush_shell_exec(dt('@drush cvapi system.flush', array('@drush' => $drush_cmd)))) {
    if ($verbose) {
      drush_log('Flushed CiviCRM caches.', 'ok');
    }
  }
  else {
    $message[] = dt('Unable to flush CiviCRM caches.');
    $message += drush_shell_exec_output();
    return drush_set_error('DRUSH_FUZION_CIVICRM_CACHEFLUSH_FAILED', implode(PHP_EOL, $message));
  }

  if (drush_shell_exec(dt('@drush cc all', array('@drush' => $drush_cmd)))) {
    if ($verbose) {
      drush_log('Flushed Drupal caches.', 'ok');
    }
  }
  else {
    $message[] = dt('Unable to flush Drupal caches.');
    $message += drush_shell_exec_output();
    return drush_set_error('DRUSH_FUZION_DRUPAL_CACHEFLUSH_FAILED', implode(PHP_EOL, $message));
  }

  if (drush_shell_exec(dt('@drush arch-send', array('@drush' => $drush_cmd)))) {
    if ($verbose) {
      drush_log('Reported to Archimedes.', 'ok');
    }
  }
  else {
    drush_log(dt('Archimedes update failed.'), 'warning');
  }

  if (drush_shell_exec('git push')) {
    if ($verbose) {
      drush_log('Pushed to default Git remote.', 'ok');
    }
  }
  else {
    drush_log(dt('Git push failed.'), 'warning');
  }
  $args = array(
    '@old_release' => $code_version,
    '@release' => $release,
    '@path' => $root,
  );
  drush_log(dt('Upgraded CiviCRM from @old_release to @release in @path.', $args), 'ok');
}

/**
 * Extract the latest stable release from CiviCRM's versions.json.
 */
function _drush_fuzion_find_latest_stable($versions) {
  foreach ($versions as $k => $data) {
    if ($data->status == 'stable') {
      $release = array_pop($data->releases);
      return $release->version;
    }
  }
}
