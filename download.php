<?php

$versions = array(
  '1.5' => 'dist/UMWP_Autochanger_1.5_Setup.exe',
  '1.6' => 'dist/UMWP_Autochanger_1.6_Setup.exe',
  '1.7' => 'dist/UMWP_Autochanger_1.7_Setup.exe',
  '1.8' => 'dist/UMWP_Autochanger_1.8_Setup.exe',
  '1.9' => 'dist/UMWP_Autochanger_1.9_Setup.exe',
  '2.0' => 'dist/UMWP_Autochanger_2.0_Setup.exe',
  '2.1' => 'dist/UMWP_Autochanger_2.1_Setup.exe',
  '2.2' => 'dist/UMWP_Autochanger_2.2_Setup.exe',
  '2.3.0' => 'dist/UMWP_Autochanger_2.3.0_Setup.exe',
  '2.3.1' => 'dist/UMWP_Autochanger_2.3.1_Setup.exe',
  '2.3.2' => 'dist/UMWP_Autochanger_2.3.2_Setup.exe',
  '2.4.0' => 'dist/UMWP_Autochanger_2.4.0_Setup.exe',
);

if (empty($_GET['ver']) || $_GET['ver'] == 'latest' || !isset($versions[$_GET['ver']]))
{
  $keys = array_keys($versions);
  $_GET['ver'] = array_pop($keys);
}

$json = json_decode(file_get_contents('downloads.json'), true);

if (!isset($json[$_GET['ver']]))
{
  $json[$_GET['ver']] = 0;
}
$json[$_GET['ver']]++;

file_put_contents('downloads.json', json_encode($json));

$file = $versions[$_GET['ver']];

if (isset($_GET['serve']))
{
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
  header('Location: http://www.strangeplanet.fr/work/umwp-autochanger/'.$file);
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