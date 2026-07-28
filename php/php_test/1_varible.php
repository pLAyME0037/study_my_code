<?php

define("<?php", "{{");
define("?>", "}}");

$i = 0;

printf($i + 1);
?>

<!DOCTYPE html>
<html>
    <head>
        <title>My 1 html</title>
    </head>
    <body>
        ello writing
        {{ echo "Hello, World!\n" }}
        <?php echo "Hello, World!\n" ?>
        <?php echo $i . "\n" ?>
    </body>
</html>
