<?php

/**
 * @file
 *   The hosting feature definition for hosting_saas_utils.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_saas_utils_hosting_feature() {
  $features['saas_utils'] = array(
    // title to display in form
    'title' => t('SaaS Utilities'),
    // description
    'description' => t('Does misc SaaS stuff, mostly set variables given by POST.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_saas_utils',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental',
  );

  return $features;
}
