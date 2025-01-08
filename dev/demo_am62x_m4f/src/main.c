/*
 * Copyright (c) 2024 Valeo
 *
 *
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier */
#define LED0_NODE DT_ALIAS(led0)
#define RELAY_CTL0_NODE DT_ALIAS(relayctl0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec relayctl = GPIO_DT_SPEC_GET(RELAY_CTL0_NODE, gpios);

int init_led0(void)
{
	int ret;

	printf(" * start led0 init\n");

	if (!gpio_is_ready_dt(&led)) {
		printf("error: gpio led is not ready\n");
		return 0;
	}
	else {
		printf("success: gpio led is ready\n");
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
	 	printf("error: gpio_pin_configure_dt led failed\n");
		return 0;
	}
	else {
		printf("success: gpio_pin_configure_dt led success\n");
	}

	return 1;
}

int init_relayctl(void)
{
	int ret;

	if (!gpio_is_ready_dt(&relayctl)) {
		printf("error: gpio relayctl is not ready\n");
		return 0;
	}
	else {
		printf("success: gpio relayctl is ready\n");
	}

	ret = gpio_pin_configure_dt(&relayctl, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
	 	printf("error: gpio_pin_configure_dt relayctl failed\n");
		return 0;
	}
	else {
		printf("success: gpio_pin_configure_dt relayctl success\n");
	}

	return 1;
}

int init_hw(void)
{
	// SOM LED
	if(!init_led0()) {
		printf("error: init_led0 failed\n");
		return 0;
	}
	else {
		printf("success: init_led0 success\n");
	}

	// RELAY CONTROL
	if(!init_relayctl()) {
		printf("error: init_relayctl failed\n");
		return 0;
	}
	else {
		printf("success: init_relayctl success\n");
	}

	return 1;
}

int main(void)
{
	int ret;
	bool led_state = true;
	bool relayctl_state = true;

	printf("Demo project for M4F cores of AM62x SoC - V0.1.0\n");

	if(!init_hw()) {
		printf("error: init_hw failed\n");
		return 0;
	}
	else {
		printf("success: init_hw success\n");
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			printf("error: gpio_pin_toggle_dt led failed\n");
			return 0;
		}

		ret = gpio_pin_toggle_dt(&relayctl);
		if (ret < 0) {
			printf("error: gpio_pin_toggle_dt relayctl failed\n");
			return 0;
		}
		led_state = !led_state;
		relayctl_state = led_state;

		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
