<?php
/**
 * Migration script for clothes_app database.
 * Usage:
 *   php migrate.php              # create tables if missing
 *   php migrate.php --reset      # drop + recreate all tables
 *   php migrate.php --seed       # only seed admin user
 */

require_once __DIR__ . '/connection.php';

$isReset = in_array('--reset', $argv ?? []);
$isSeedOnly = in_array('--seed', $argv ?? []);

$isCLI = php_sapi_name() === 'cli';

function out($msg) {
    global $isCLI;
    if ($isCLI) {
        echo $msg . PHP_EOL;
    } else {
        echo "<div>" . htmlspecialchars($msg) . "</div>\n";
    }
}

function run($sql) {
    global $connectNow;
    if (!$connectNow->query($sql)) {
        out("ERROR: " . $connectNow->error);
        return false;
    }
    return true;
}

if (!$isCLI) {
    echo "<pre style='font-family:monospace;background:#1e1e1e;color:#00ff88;padding:20px;'>\n";
}

out("=== Migration: clothes_app ===\n");

if (!$connectNow->select_db('clothes_app')) {
    run("CREATE DATABASE IF NOT EXISTS clothes_app");
    $connectNow->select_db('clothes_app');
    out("Database 'clothes_app' created.");
}

$tables = [
    'users_table' => "
        CREATE TABLE IF NOT EXISTS users_table (
            user_id       INT AUTO_INCREMENT PRIMARY KEY,
            user_name     VARCHAR(255) NOT NULL,
            user_email    VARCHAR(255) NOT NULL UNIQUE,
            user_password VARCHAR(255) NOT NULL
        )
    ",

    'items_table' => "
        CREATE TABLE IF NOT EXISTS items_table (
            item_id     INT AUTO_INCREMENT PRIMARY KEY,
            name        VARCHAR(255) NOT NULL,
            rating      DECIMAL(3,2) DEFAULT 0,
            tags        TEXT,
            price       DECIMAL(10,2) NOT NULL,
            sizes       TEXT,
            colors      TEXT,
            description TEXT,
            image       VARCHAR(255)
        )
    ",

    'cart_table' => "
        CREATE TABLE IF NOT EXISTS cart_table (
            cart_id   INT AUTO_INCREMENT PRIMARY KEY,
            user_id   INT NOT NULL,
            item_id   INT NOT NULL,
            quantity  INT NOT NULL DEFAULT 1,
            color     VARCHAR(100),
            size      VARCHAR(100),
            FOREIGN KEY (user_id) REFERENCES users_table(user_id) ON DELETE CASCADE,
            FOREIGN KEY (item_id) REFERENCES items_table(item_id) ON DELETE CASCADE
        )
    ",

    'favorite_table' => "
        CREATE TABLE IF NOT EXISTS favorite_table (
            user_id INT NOT NULL,
            item_id INT NOT NULL,
            PRIMARY KEY (user_id, item_id),
            FOREIGN KEY (user_id) REFERENCES users_table(user_id) ON DELETE CASCADE,
            FOREIGN KEY (item_id) REFERENCES items_table(item_id) ON DELETE CASCADE
        )
    ",

    'orders_table' => "
        CREATE TABLE IF NOT EXISTS orders_table (
            order_id        INT AUTO_INCREMENT PRIMARY KEY,
            user_id         INT NOT NULL,
            selectedItems   TEXT NOT NULL,
            deliverySystem  VARCHAR(100),
            paymentSystem   VARCHAR(100),
            note            TEXT,
            totalAmount     DECIMAL(10,2) NOT NULL,
            image           VARCHAR(255),
            status          VARCHAR(50) NOT NULL DEFAULT 'new',
            shipmentAddress TEXT,
            phoneNumber     VARCHAR(50),
            dateTime        TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users_table(user_id) ON DELETE CASCADE
        )
    ",

    'admins_table' => "
        CREATE TABLE IF NOT EXISTS admins_table (
            admin_id       INT AUTO_INCREMENT PRIMARY KEY,
            admin_email    VARCHAR(255) NOT NULL UNIQUE,
            admin_password VARCHAR(255) NOT NULL
        )
    ",
];

if ($isReset) {
    out("--reset: dropping all tables...");
    $dropOrder = array_reverse(array_keys($tables));
    foreach ($dropOrder as $name) {
        run("DROP TABLE IF EXISTS $name");
        out("  DROPPED $name");
    }
    out("");
}

if ($isSeedOnly) {
    out("--seed mode: skipping table creation.\n");
} else {
    foreach ($tables as $name => $sql) {
        if (run($sql)) {
            $result = $connectNow->query("SHOW TABLES LIKE '$name'");
            out(($result && $result->num_rows > 0 ? "  \xE2\x9C\x93 created" : "  \xE2\x9C\x93 already exists") . "  $name");
        }
    }
}

out("");

// Seed default admin
$result = $connectNow->query("SELECT COUNT(*) AS cnt FROM admins_table");
$row = $result->fetch_assoc();
if ($row['cnt'] == 0) {
    $email = 'admin@admin.com';
    $pass = md5('admin123');
    run("INSERT INTO admins_table (admin_email, admin_password) VALUES ('$email', '$pass')");
    out("  \xE2\x9C\x93 seeded default admin: admin@admin.com / admin123");
} else {
    out("  \xE2\x80\xA2 admin already exists, skipping seed.");
}

out("\n=== Done ===");

if (!$isCLI) {
    echo "</pre>";
}

$connectNow->close();
