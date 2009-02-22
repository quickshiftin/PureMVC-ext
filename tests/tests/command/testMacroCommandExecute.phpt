--TEST--
MacroCommand::execute(), ensure calls to each commands execute methods are invokded
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
include(dirname(__FILE__) . '/../testlib/include.php');
SubCommand::enableEcho();
MyMacroCommand::setNumCommands(2);
$macroCmd = new MyMacroCommand();
$macroCmd->execute(new MyNotification());
?>
--EXPECT--
1
2
