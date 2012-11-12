wowmapper
=========

World of Warcraft MAP ParsER

wowmapper is a hobby project which started in May 2010. It was one, if not the first publicly available library to extract WoW's map data. Version v0.1 and v0.2 supported WoW v3.3.x and WoW v4.x for a long time. Since I have been pretty busy for the last two years and were absent from WoW itself too, I've stopped working on it...till now. I'm going to rewrite the whole code base and I want to make sure beginners, curious people and even advanced users have a graphical user interface to ease alot of work. Thanks to all the people that helped build the early versions and all the support I received from the community. Thank you!

wowmapper uses a couple of external libraries to do its job.
Right now it's using the following:
- FTLK (v1.3.0) is a cross-platform, lightweight GUI library. See http://www.fltk.org
- StormLib (v8.20) enables wowmapper to access MPQ files to extract its content. See http://www.zezula.net/en/mpq/stormlib.html
- GLM (v0.9.3.4) is my favorite OpenGL math library. See http://glm.g-truc.net/
- Boost (v1.46.1). See http://www.boost.org/
- Irrlicht (v1.8) is a very flexible and easy to use engine. See http://irrlicht.sourceforge.net/
- libsquish (v1.11) is able to deal with DXT1/3/5 images. See http://code.google.com/p/libsquish/
