This is an attempt to make fq_codel faster and more relevant

It is intended as a drop-in replacement for the existing
fq_codel code in the stack. As development procedes either
feeding back these changes into the linux mainline can happen
or we can fork it to be more suitable for the openwrt audience

There were also mistakes made in early development,
tracking stats that aren't useful, and new features
added that also weren't used, and since the major
use of fq_codel is in sqm systems, adding in an
fast integral shaper is a goal.

