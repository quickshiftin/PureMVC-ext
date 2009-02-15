--TEST--
SimpleCommand::__construct(), ensure __construct assigns the instance of Facade in $tihs->facade
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
var_dump(new SimpleCommand());
?>
--EXPECT--
object(SimpleCommand)#1 (1) {
  ["facade:protected"]=>
  object(Facade)#2 (3) {
    ["model:protected"]=>
    object(Model)#3 (1) {
      ["proxyMap:protected"]=>
      NULL
    }
    ["view:protected"]=>
    object(View)#5 (2) {
      ["mediatorMap:protected"]=>
      NULL
      ["observerMap:protected"]=>
      NULL
    }
    ["controller:protected"]=>
    object(Controller)#4 (2) {
      ["view:protected"]=>
      NULL
      ["commandMap:protected"]=>
      NULL
    }
  }
}
