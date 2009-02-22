<?php
function puremvc_phpt_autoload($classname) {
	$classpath = dirname(__FILE__) . "/$classname.php";

	if(!file_exists($classpath))
		trigger_error(
			"could not find class @ $classpath",
			E_USER_ERROR
		);
	else
		require_once($classpath);
}
spl_register_autload('puremvc_phpt_autoload');
?>
