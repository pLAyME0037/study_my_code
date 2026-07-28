class Main {
    public static void main(String[] args) {
        Device phone = new Phone("Apple");
        Device laptop = new Computer("Lenovo");
        Device tv = new TV("Samsung");

        phone.activate();
        laptop.activate();
        tv.activate();

        // phone.deactivate();
        // laptop.deactivate();
        // tv.deactivate();
    }
}

class Device {
    String brand;
    String powerOn;

    Device(String brand) {
        this.brand = brand;
        this.powerOn = "false";
    }

    void activate() {
        powerOn = "on";
        System.out.format("Device name is %s. Power status is %s. ", brand, powerOn);
    }

    void deactivate() {
        powerOn = "off";
        System.out.println(brand + " deactivated. Power status is " + powerOn);
    }
}

class Phone extends Device {
    Phone(String brand) {
        super(brand);
    }

    @Override
    void activate() {
        super.activate();
        System.out.println("Calling...");
    }
}

class Computer extends Device {
    Computer(String brand) {
        super(brand);
    }

    @Override
    void activate() {
        super.activate();
        System.out.println("Booting Linux...");
    }
}

class TV extends Device {
    TV(String brand) {
        super(brand);
    }

    @Override
    void activate() {
        super.activate();
        System.out.println("Displaying channel...");
    }
}

