# E-Ink-Teabook

A Teabook with a built-in E-ink display that displays self-help, compliments, and funny texts every other day.

## Hardware Design

The E-Ink-Teabook is designed to be compatible with the following hardware components:

- Arduino Pro Mini 3.3V 8MHz: It is recommended to remove the power LED for safety. Optionally, the voltage regulator can be removed, although it is less secure than leaving it intact.

- Power Source: The system can be powered by 2 AA batteries. However, for optimal performance and system stability, it is highly recommended to use a 4.5V power source such as 3 AA or 3 AAA batteries. Using lower voltage power sources may result in system instability, with the system failing at around 2.7V. The device is very low power with around 31uA current usage when "sleeping". This is thanks to the hardware design of Adafruit and my software optimization.

- Display: The Teabook features the Adafruit ThinkInk Mono 2.13-inch monochrome display in black and white. While this display provides excellent performance, there are more cost-effective alternatives available. Consider using the Adafruit E-ink Friend board for a budget-friendly solution. Flexible displays are also an option, but note that the font size may be slightly larger compared to non-flexible displays when using Adafruit libraries.

By combining the joy of tea with inspirational and humorous messages, the E-Ink-Teabook offers a unique and refreshing experience for daily use.

### Other designs

It is of course possible to use this project for other designs as well. You can think of multiple options:
- Daily motivation "post it"
- Daily fun facts painting
- Name tag
