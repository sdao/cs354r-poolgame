noinst_HEADERS = MinimalOgre.h Component.h GameObject.h Physics.h PhysicsSphereCollider.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11
assignment2_SOURCES = MinimalOgre.cpp GameObject.cpp Physics.cpp PhysicsSphereCollider.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
