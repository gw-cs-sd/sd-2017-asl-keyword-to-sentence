myoSign
=============

ASL Keyword to Sentence is an interface to help making signing easier for individual ASL users and convert a string of keywords into full sentences. The first component involves using a hand-gesture recognition device (the Myo Armband) to learn ASL signs and be able to convert these gestures into text. The second component involves converting keywords to full sentences (i.e. "take dog outside" will convert to "I will take the dog outside"). Together these two components will help ASL signers to make any ASL-text conversion much easier.

Libraries/Frameworks
-------

Here is a list of the library and frameworks used to run this program.

* [Nick Gillian's GRT](http://www.nickgillian.com/wiki/pmwiki.php/GRT/GestureRecognitionToolkit) 
* [Myo Armband](http://myo.com)
* [Myo SDK/Framework](https://developer.thalmic.com/docs/api_reference/platform/the-sdk.html)

Structure
-------
* `get` the module to get the data from the Myo armband
* `analyze`	the module to traing the data from training module
* `myoSign`	the module that uses the device listener to give real time prediction of the gesture
* `gesturelistener.h` the new device listener that checks for new gestures taught to the armband
* `myoSignApp` app that is used to train the `get` module **IN PROGRESS**
* `displayWords` app that is used to display the current word signed
* `myoTrain` app that is used to display and analyze the data in the folders

Installation/Usage
-----------

Still in progress.
