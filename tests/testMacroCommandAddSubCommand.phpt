--TEST--
MacroCommand::addSubCommand(), ensure addSubCommand actually stores the ICommand in an internal array
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
class SubCommand {}
class MyMacroCommand extends MacroCommand {
	public function __construct() {
		parent::__construct();
	}
	protected function initializeMacroCommand() {
		$this->addSubCommand(new SubCommand());
	}
}
$macroCmd = new MyMacroCommand();
var_dump($macroCmd);
?>
--EXPECT--
object(MyMacroCommand)#1 (2) {
  ["facade:protected"]=>
  object(Facade)#3 (3) {
    ["model:protected"]=>
    object(Model)#4 (1) {
      ["proxyMap:protected"]=>
      NULL
    }
    ["view:protected"]=>
    object(View)#6 (2) {
      ["mediatorMap:protected"]=>
      NULL
      ["observerMap:protected"]=>
      NULL
    }
    ["controller:protected"]=>
    object(Controller)#5 (2) {
      ["view:protected"]=>
      NULL
      ["commandMap:protected"]=>
      NULL
    }
  }
  ["subCommands:private"]=>
  array(1) {
    [0]=>
    object(SubCommand)#2 (0) {
    }
  }
}
