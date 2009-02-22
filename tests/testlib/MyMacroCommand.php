<?php
class MyMacroCommand extends MacroCommand {
	private static $numCommands = 1;

	public static function setNumCommands($numCommands) {
		self::$numCommands = (int) $numCommands;
	}

	protected function initializeMacroCommand() {
		for($i=0; $i<self::$numCommands; $i++)
			$this->addSubCommand('SubCommand');
	}
}
