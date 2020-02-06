--TEST--
DOMNode::after()
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once("dom_test.inc");

$dom = new DOMDocument;
$dom->loadXML('<test><mark>first</mark><mark>second</mark></test>');
if(!$dom) {
  echo "Error while parsing the document\n";
  exit;
}

$element = $dom->documentElement->firstElementChild;
$secondMark = $dom->documentElement->lastElementChild;

$element->after(
  'text inserted after',
  $dom->createElement('inserted-after', 'content')
);

$secondMark->after('text inserted after second');

print_node_list_compact($dom->documentElement->childNodes);
?>
--EXPECT--
<mark>
  first
</mark>
text inserted after
<inserted-after>
  content
</inserted-after>
<mark>
  second
</mark>
text inserted after second
