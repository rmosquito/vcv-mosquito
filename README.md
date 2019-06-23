# vcv-mosquito
Plugins for the VCV soft-synth.

These are based on some Atmel-based physical modules I made a number of years ago.  The software versions here replicate that crunchy 8-bit aliased sound of the originals, but use floats instead of integer math and bit shifts.  Hopefully the hilarious backwards-ness of this amuses you as much as it amused me.  I had not originally intended to post these here due to code-shame, but it seemed the easiest way to make it available. Eventually I'll clean the code up and strike that last sentence.  ;)

![screenshot](https://community.vcvrack.com/uploads/default/original/2X/b/bfcb36c6a072f083912b006cfad08d220a8b1482.png)

## phaseCzar

phaseCzar is inspired by Casio's phase distortion method. This Wikipedia article summarizes the technique nicely, and includes a link to the (now defunct) patent:
https://en.wikipedia.org/wiki/Phase_distortion_synthesis

At it’s core it’s a DDS-based phase distortion oscillator. You can think of this as a main oscillator (the bottom one) and two other oscillators hard sync’d to it. The frequency and amplitude of each oscillator is under voltage control. Tuning the two upper oscillators higher than the main oscillator’s frequency and then modulating those frequencies should produce good fun.

Hopefully it’s apparent from the lines on the panel, but if the oscillator buttons on the left hand side are lit up, that means the oscillator in question is tied to the v/oct input. As I said, I’m more interested in pitched sounds, so I’ve never actually unlinked the bottom oscillator… but maybe you’ll want to for something? Let me know if that button should stay or go.

The “waveform” knob (and CV input) switches between 16 (8-bit) waveforms. The interpolate button lets you interpolate between those waveforms, wavetable synth style. I like the harshness of no interpolation, but you may like interpolated… niceness.

RIGHT NOW, if all the amplitude knobs are all at 12 o’clock, the signal should be clean. Turning them past 12 will result in wavefolding. The "Boost" button makes that easier to avoid or exploit, depending on your prefernce. 

Tip: turning down the main (lower) oscillator all the way and leaving the other two oscillator up (especially when unlinked from v/oct leads to very formatanty, VOSIM type sounds. Because… that’s basically VOSIM.

The “smooth” knob (and CV input) is the most subtle bit. Part of the original Casio spec for PD involved applying an envelope so that the harsh sync sounds are attenuated. Counter clockwise, you get the maximum harsh sync sound. (In my physical module, I just had a switch, but I thought a jack might be useful for more evolving, subtle sounds. Not really my hat, but it may be yours.)

## pulseCzar 

pulseCzar is inspired by Curtis Roads' pulsar synthesis.  You can read his original article on the technique here:
https://www.curtisroads.net/s/SoundCompwithPulsars.  Also, check out his book _Microsound_.  Fipping through that paper is a good way to get an idea of what's going on here.  It's weird, but think of the "Envelope Pitch" as the master pitch.  The "Waveform Pitch" is more of the timbral color.  As above, there are different waveforms and envelope shapes to choose from.

Unlike phaseCzar, pulseCzar only has 8 waveshapes (and 8 envelope shapes).  This was due to the memory limitations of the ATMEGA chips I used on the hardware modules and uh... I didn't make it better here.

Integral to Roads' method was the idea of masking -- certain pulses would be skipped.  THe most novel of these (IMO) is what Roads called "Stochastic Masking."  Basically, you drop random pulses.  So if, for instance, you applied an envelope with a long attack to the "Stochastic Masking" parameter, your sound would slowly fall apart.  I'll implement more varieties of masking in v2.

Obviously, the gain knob applies again.  As with phaseCzar, going over +/-5V will result in a similar method of wavefolding.

I'd like to give a special nod to @Coirt, as he provided invaluable helpping me figure out how to get them migrated to the 1.0 API.  I hope you enjoy these, and if you'd like to produce real documentation for the modules, please do so in the wiki.

Thanks!
-RM
