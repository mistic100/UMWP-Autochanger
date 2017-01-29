<?php

$versions = array(
  'zip' => array(
    '1.0a1' => 'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.0-alpha1.zip',
    '1.0a2' => 'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.0-alpha2.zip',
    '1.0' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.0.zip',
    '1.1' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.1.zip',
    '1.2' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.2.zip',
    '1.3' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.3.zip',
    '1.4' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.4.zip',
    '1.5' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.5.zip',
    '1.6' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.6.zip',
    '1.7' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.7.zip',
    '1.8' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.8.zip',
    '1.9' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.9.zip',
    '2.0' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.0.zip',
    '2.1' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.1.zip',
    '2.2' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.1.zip',
    ),
  
  'exe' => array(
    '1.5' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.5_Setup.exe',
    '1.6' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.6_Setup.exe',
    '1.7' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.7_Setup.exe',
    '1.8' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.8_Setup.exe',
    '1.9' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_1.9_Setup.exe',
    '2.0' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.0_Setup.exe',
    '2.1' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.1_Setup.exe',
    '2.2' =>   'http://www.strangeplanet.fr/work/umwp-autochanger/dist/UMWP_Autochanger_2.2_Setup.exe',
    ),
  );
  
if (empty($_GET['type']) || $_GET['type'] != 'zip')
{
  $_GET['type'] = 'exe';
}

if (empty($_GET['ver']) || $_GET['ver'] == 'latest')
{
  $keys = array_keys($versions['exe']);
  $_GET['ver'] = array_pop($keys);
}

if (!isset($versions[$_GET['type']][$_GET['ver']]))
{
  if (isset($versions['zip'][$_GET['ver']]))
  {
    $_GET['type'] = 'zip';
  }
  else
  {
    $keys = array_keys($versions['exe']);
    $_GET['ver'] = array_pop($keys);
  }
}

$json = json_decode(file_get_contents('downloads.json'), true);

if (!isset($json[$_GET['ver']]))
{
  $json[$_GET['ver']] = 0;
}
$json[$_GET['ver']]++;

file_put_contents('downloads.json', json_encode($json));

$file = $versions[$_GET['type']][$_GET['ver']];

if (isset($_GET['serve']))
{
  $file = str_replace('http://www.strangeplanet.fr/work/umwp-autochanger/', '', $file);
  
  header('Content-Type: application/force-download; name="'.basename($file).'"');
  header('Content-Disposition: attachment; filename="'.basename($file).'"');
  header('Content-Description: File Transfer');
  header('Content-Transfer-Encoding: binary');
  header('Content-Length: '.filesize($file).'');

  header('Cache-Control: no-cache, must-revalidate');
  header('Pragma: no-cache');
  header('Expires: 0');

  readlargefile($file);
}
else
{
  header('Location: '.$file);
}

function readlargefile($fullfile)
{
  $fp = fopen($fullfile, 'rb');

  if ($fp)
  {
    while (!feof($fp))
    {
      print(fread($fp, 2097152));
    }

    fclose($fp);
  }
}