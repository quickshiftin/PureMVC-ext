--TEST--
MacroCommand::execute(), ensure calls to each commands execute methods are invokded
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
class SubCommand {
	var $id = -1;
	public function __construct($id) {
		$this->id = $id;
	}
	public function execute() {
		echo $id . PHP_EOL;
	}
}
class MyNotification implements INotification {
	public function getName() {}
	public function getType() {}
	public function getBody() {}
	public function setName($name) {}
	public function setBody($body) {}
	public function setType($type) {}
	public function toString() {}
}
$subCmd1 = new SubCommand(1);
$subCmd2 = new SubCommand(2);
$macroCmd = new MacroCommand();
$macroCmd->addSubCommand($subCmd1);
$macroCmd->addSubCommand($subCmd2);
$macroCmd->execute(new MyNotification());
?>
--EXPECT--
12
