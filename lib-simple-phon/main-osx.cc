#include <iostream>
#include <CoreAudio/CoreAudio.h>

int main (int, char*[]) {
  AudioObjectPropertyAddress propertyAddress = {
    kAudioHardwarePropertyDevices,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMaster
  };

  UInt32 dataSize = 0;

  OSStatus status = AudioObjectGetPropertyDataSize(
    kAudioObjectSystemObject,
    &propertyAddress,
    0,
    NULL,
    &dataSize
  );

  if (kAudioHardwareNoError != status) {
    fprintf(stderr, "AudioObjectGetPropertyDataSize (kAudioHardwarePropertyDevices) failed: %i\n", status);
    return 0;
  }

  AudioDeviceID *audioDevices = static_cast<AudioDeviceID*>(malloc(dataSize));
  
  if (audioDevices == NULL) {
    fputs("Unable to allocate memory", stderr);
    return 1;
  }

  status = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize, audioDevices);

  if (status != kAudioHardwareNoError) {
    fprintf(stderr, "AudioObjectGetPropertyData (kAudioHardwarePropertyDevices) failed: %i\n", status);
    free(audioDevices), audioDevices = NULL;
    return 1;
  }

  int deviceCount = static_cast<int>(dataSize / sizeof(AudioDeviceID));

  for (int i = 0; i < deviceCount; ++i) {
    // Query device UID
    CFStringRef deviceUID = NULL;
    dataSize = sizeof(deviceUID);
    propertyAddress.mSelector = kAudioDevicePropertyDeviceUID;
    status = AudioObjectGetPropertyData(audioDevices[i], &propertyAddress, 0, NULL, &dataSize, &deviceUID);

    if (kAudioHardwareNoError != status) {
      fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceUID) failed: %i\n", status);
      continue;
    }

    // Query device name
    CFStringRef deviceName = NULL;
    dataSize = sizeof(deviceName);
    propertyAddress.mSelector = kAudioDevicePropertyDeviceNameCFString;
    status = AudioObjectGetPropertyData(audioDevices[i], &propertyAddress, 0, NULL, &dataSize, &deviceName);

    if (kAudioHardwareNoError != status) {
      fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceNameCFString) failed: %i\n", status);
      continue;
    }

    // Query device manufacturer
    CFStringRef deviceManufacturer = NULL;
    dataSize = sizeof(deviceManufacturer);
    propertyAddress.mSelector = kAudioDevicePropertyDeviceManufacturerCFString;
    status = AudioObjectGetPropertyData(audioDevices[i], &propertyAddress, 0, NULL, &dataSize, &deviceManufacturer);

    if (kAudioHardwareNoError != status) {
      fprintf(stderr, "AudioObjectGetPropertyData (kAudioDevicePropertyDeviceManufacturerCFString) failed: %i\n", status);
      continue;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "Device uid          : " << CFStringGetCStringPtr(deviceManufacturer, kCFStringEncodingUTF8) << std::endl;
    std::cout << "Device Manufacturer : " << CFStringGetCStringPtr(deviceManufacturer, kCFStringEncodingUTF8) << std::endl;
    std::cout << "Device Name         : " << CFStringGetCStringPtr(deviceName, kCFStringEncodingUTF8) << std::endl;

    CFRelease(deviceUID);
    CFRelease(deviceManufacturer);
    CFRelease(deviceName);
  }

	return 0;
}
