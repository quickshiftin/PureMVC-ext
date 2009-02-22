<?php
class SubCommand {
	static $id = 0;
	private static $shouldEcho = false;

	public static function enableEcho() {
		self::$shouldEcho = true;
	}

	public function __construct() {
		self::$id++;
	}

	public function execute() {
		if(self::$shouldEcho)
			echo self::$id . PHP_EOL;
	}
}
