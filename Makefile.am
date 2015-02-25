noinst_HEADERS = MinimalOgre.h Component.h RootGameObject.h GameObject.h \
		 Physics.h PhysicsCollider.h PhysicsSphereCollider.h Player.h \
		 GameEnums.h PhysicsBoxCollider.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11
assignment2_SOURCES = MinimalOgre.cpp RootGameObject.cpp GameObject.cpp \
	              PhysicsSphereCollider.cpp PhysicsCollider.cpp \
		      Physics.cpp Player.cpp PhysicsBoxCollider.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
