<?php

$file = json_decode(file_get_contents('downloads.json'), true);

switch ($_GET['what'])
{
case 'version':
  $keys = array_keys($file);
  $version = array_pop($keys);
  $url = 'https://img.shields.io/badge/stable%20version-' . $version . '-brightgreen.svg?style=flat-square';
  break;
  
case 'downloads':
  $downloads = array_sum($file);
  if ($downloads >= 1000)
  {
    $downloads = number_format($downloads/1000, 1) . 'k';
  }
  $url = 'https://img.shields.io/badge/downloads-' . $downloads . '-blue.svg?style=flat-square';
  break;

default:
  $url = 'https://img.shields.io/badge/undef-error-red.svg?style=flat-square';
}

header('Location: '.$url);