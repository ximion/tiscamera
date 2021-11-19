# Upgrade Guide for 1.0

To upgrade to tiscamera 1.0 you have to be aware of the following changes:

## Compilation

tiscamera-1.0 uses a more up to date version of aravis.
This aravis version requires meson >= ????? for compilation.

Ubuntu LTS 18.04 is not capable of compiling this out of the box.

You can set the cmake option `TCAM_DOWNLOAD_MESON` to `ON` to have tiscamera handle this case for you.

The configuration flags `BUILD_ARAVIS` and `BUILD_DOCUMENTATION` now have the default `ON`.


## Device discovery

Device discovery is no longer part of tcamprop.
Instead the GstDevice functionality is used.

For sample code on how to implement this feature look at the examples folder.

## GStreamer

### tcambin

The property `use-dutils` has been replaced by the enum `conversion-element`.

To explicitly use tcamdutils set `conversion-element` to `tcamdutils`.
Default is `auto`.

The property `state` requires the element to be at least in the `GST_STATE_READY` state.
This is to ensure that a device has been opened.

### Removed elements

tcamwhitebalance, tcamautofocus, tcamautoexposure have been removed.
The functionality is now part of tcammainsrc/tcampimipisrc/tcamtegrasrc.

Application of whitebalance is done via tcamconvert, tcamdutils or tcamdutils-cuda.

### Device opening

tcamsrc has to be in `GST_STATE_READY` for a device to be opened.

## Properties

All properties name/types are now genicam compliant.
That means all camera models/types have identical property names and identical property types.

### Names

All property names are now CamelCase.

For a human readable name call `tcam_property_base_get_display_name()`;

### Flags

Property Flags are now actively used to communicate information to the user.

If you would like a complete overview over our flags, please take a look at our documentation: TODO: Add ???? link

- Locked
  Locked properties will refuse any set_value call as another property has control over it.
  This is typically the case for auto properties, e.g. ExposureAuto locks ExposureTime.
  To manually set ExposureTime you will have to set ExposureAuto to 'Off'.

The most important property changes are:

### Exposure

The property will always have the name "ExposureTime". It will always be a float.
Its unit will always be µs.

### Gain

The property will always have the name "Gain". It will always be a float.
Its unit will always be dB.

### GainAuto/ExposureAuto

It will always be an enum.

### Binning / Skipping / OverrideScanningMode

OverrideScanningMode is now correctly implemented.

Binning, Skipping and Override Scanning Mode are not available as properties to the user.
They are part of the caps description.
See GstCaps in this document.

### BalanceRatio / BalanceRatioRaw / BalanceRatioSelector

Abstracted to `BalanceWhiteRed`, `BalanceWhiteGreen` and `BalanceWhiteBlue`.

### BalanceWhiteRed / BalanceWhiteGreen / BalanceWhiteBlue

These values are always float. The will always have a value range of 0.0 - 0.4.
To get the range 0 - 265 mulptily value with 64.

## tcamprop

All tcamprop functions now have an GError argument.
This allows for better error handling.
Please look at the corresponding examples on how to use them.

## GstCaps

GstCaps have been expanded with the following fields:

### binning

GstCaps can now contain the value 'binning'.
If you have not yet used binning this does not affect you.

See documentation for further details ???? link

### skipping

GstCaps can now contain the value skipping'.
If you have not yet used skipping this does not affect you.

See documentation for further details ???? link

## TcamProp

The actual API has been rewritten and is now GeniCam compatible.

All properties are now represented as objects.

TODO: documentation link
TODO sample references


## tools

### tcam-ctrl

#### tcam-ctrl --version

The version output now also includes a module string identifying the modules tiscamera has been configured/compiled with.

#### tcam-ctrl -p

Output has changed to include additional information and to increase readability.
Contains new fields to accommodate tcamprop rewrite.

### tcam-capture

tcam-capture has been rewritten in C++.

#### Saving images/videos

Removed. Write own script or use tis-measure as replacement TODO: ???? link to tis-measure

#### camera-ip-conf

Removed. Use tcam-gigetool.

#### firmware-upload

Moved to own repository. See https://github.com/TheImagingSource/tcam-firmware-update

## environment

### TCAM_LOG

Removed. Set GST_DEBUG=tcam-libtcam:X to get the output.

### TCAM_ARV_PACKET_REQUEST_RATIO

Removed. Set environment variable `TCAM_ARV_STREAM_OPTIONS` to `packet-request-ratio=<float>`