/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "gsCore.h"
#include "gsPhysics.h"



gsProperty::gsProperty(gkVariable *var) : m_prop(var), m_creator(false)
{
}


gsProperty::gsProperty() : m_prop(0), m_creator(false)
{
}


gsProperty::gsProperty(const gkString &name, bool value)
{
	m_creator = true;
	m_prop = new gkVariable(name, false);
	m_prop->setValue(value);
}


gsProperty::gsProperty(const gkString &name, double value)
{
	m_creator = true;
	m_prop = new gkVariable(name, false);
	m_prop->setValue((float)value);
}



gsProperty::gsProperty(const gkString &name, const gkString &value)
{
	m_creator = true;
	m_prop = new gkVariable(name, false);
	m_prop->setValue(value);
}



gsProperty::gsProperty(const gsProperty &oth)
{
	m_creator = true;
	m_prop = new gkVariable(oth.getName(), false);
	m_prop->setValue(oth.getValue());
}


gsProperty::~gsProperty()
{
	makeDebug(false);
	if (m_creator)
		delete m_prop;
}



const gkString &gsProperty::getName(void) const
{
	static gkString localStr = "";
	if (m_prop)
		return m_prop->getName();
	return localStr;
}



const gkString &gsProperty::getValue(void) const
{
	static gkString localStr;
	if (m_prop)
		localStr = m_prop->getValueString();
	return localStr;
}



void gsProperty::makeDebug(bool v)
{
	if (m_prop && m_prop->isDebug() != v && gkEngine::getSingletonPtr())
	{
		m_prop->setDebug(v);
		if (v)
			gkEngine::getSingleton().addDebugProperty(m_prop);
		else
			gkEngine::getSingleton().removeDebugProperty(m_prop);
	}
}



gsPropertyType gsProperty::getType(void)  const
{

	if (m_prop)
	{
		switch (m_prop->getType())
		{
		case gkVariable::VAR_BOOL:
			return PROP_BOOL;
		case gkVariable::VAR_INT:
		case gkVariable::VAR_REAL:
			return PROP_NUMBER;
			// TODO
		default:
			break;
		}
		return PROP_STRING;
	}
	return PROP_NULL;
}



bool gsProperty::toBool(void)  const
{
	if (m_prop)
		return m_prop->getValueBool();
	return false;
}



double gsProperty::toNumber(void)  const
{
	if (m_prop)
		return (double)m_prop->getValueReal();
	return -1;

}



gkString gsProperty::toString(void)  const
{
	if (m_prop)
		return m_prop->getValueString();
	return "";
}



void gsProperty::fromBool(bool v)
{
	if (m_prop)
		m_prop->setValue(v);
}



void gsProperty::fromNumber(double v)
{
	if (m_prop)
		m_prop->setValue((gkScalar)v);
}



void gsProperty::fromString(const gkString &v)
{
	if (m_prop)
		m_prop->setValue(v);
}


#define PROP_INSERT_T(name, var, cast)\
    m_properties.insert(name, new gsProperty(name, (cast)m_defs->var));

#define PROP_INSERT_N(name, var) PROP_INSERT_T(name, var, double)
#define PROP_INSERT_B(name, var) PROP_INSERT_T(name, var, bool)
#define PROP_INSERT_S(name, var) PROP_INSERT_T(name, var, gkString)


static void gkVar_fromVar(gkVariable &v, bool &r)
{
	r = v.getValueBool();
}

static void gkVar_fromVar(gkVariable &v, int &r)
{
	r = v.getValueInt();
}
static void gkVar_fromVar(gkVariable &v, gkScalar &r)
{
	r = v.getValueReal();
}
static void gkVar_fromVar(gkVariable &v, gkString &r)
{
	r = v.getValueString();
}


#define PROP_SET_T(name, var, cast)\
    if (v.getName() == name )\
        gkVar_fromVar(v, (cast)m_defs->var);

#define PROP_SET_TE(name, var, cast)\
    if (v.getName() == name )\
    {\
        int c = (int)m_defs->var;\
        gkVar_fromVar(v, c);\
        m_defs->var = (cast)c;\
    }

#define PROP_SET_I(name, var) PROP_SET_T(name, var, int&)
#define PROP_SET_N(name, var) PROP_SET_T(name, var, gkScalar&)
#define PROP_SET_B(name, var) PROP_SET_T(name, var, bool&)
#define PROP_SET_S(name, var) PROP_SET_T(name, var, gkString&)
#define PROP_SET_E(name, var, E) PROP_SET_TE(name, var, E)





gsUserDefs::gsUserDefs(gkUserDefs *defs)
{
	m_defs = defs;

	PROP_INSERT_N("renderSystem",       rendersystem);
	PROP_INSERT_N("sceneManager",       sceneManager);
	PROP_INSERT_B("verbose",            verbose);
	PROP_INSERT_N("winx",               winsize.x);
	PROP_INSERT_N("winy",               winsize.y);
	PROP_INSERT_S("winTitle",           wintitle);
	PROP_INSERT_B("fullScreen",         fullscreen);
	PROP_INSERT_B("blenderMat",         blendermat);
	PROP_INSERT_B("grabInput",          grabInput);
	PROP_INSERT_B("debugFps",           debugFps);
	PROP_INSERT_B("debugPhysics",       debugPhysics);
	PROP_INSERT_B("debugPhysicsAABB",   debugPhysicsAabb);
	PROP_INSERT_B("usebulletDBVT",      useBulletDbvt);
	PROP_INSERT_B("showDebugProps",     showDebugProps);
	PROP_INSERT_B("debugSounds",        debugSounds);
	PROP_INSERT_B("enableShadows",      enableshadows);
	PROP_INSERT_S("shadowTechnique",    shadowtechnique);
	PROP_INSERT_N("colourShadowR",      colourshadow.r);
	PROP_INSERT_N("colourShadowG",      colourshadow.g);
	PROP_INSERT_N("colourShadowB",      colourshadow.b);
	PROP_INSERT_N("farDistanceShadow",  fardistanceshadow);
}




gsUserDefs::~gsUserDefs()
{
	UTsize p=0;
	for (; p< m_properties.size(); ++p)
		delete m_properties.at(p);

	m_properties.clear();
}



void gsUserDefs::setValueEvent(gkVariable &v)
{
	if (m_properties.find(gkHashedString(v.getName())) != UT_NPOS)
	{
		PROP_SET_E("renderSystem",       rendersystem, OgreRenderSystem);
		PROP_SET_I("sceneManager",       sceneManager);
		PROP_SET_B("verbose",            verbose);
		PROP_SET_N("winx",               winsize.x);
		PROP_SET_N("winy",               winsize.y);
		PROP_SET_S("winTitle",           wintitle);
		PROP_SET_B("fullScreen",         fullscreen);
		PROP_SET_B("blenderMat",         blendermat);
		PROP_SET_B("grabInput",          grabInput);
		PROP_SET_B("debugFps",           debugFps);
		PROP_SET_B("debugPhysics",       debugPhysics);
		PROP_SET_B("debugPhysicsAABB",   debugPhysicsAabb);
		PROP_SET_B("usebulletDBVT",      useBulletDbvt);
		PROP_SET_B("showDebugProps",     showDebugProps);
		PROP_SET_B("debugSounds",        debugSounds);
		PROP_SET_B("enableShadows",      enableshadows);
		PROP_SET_S("shadowTechnique",    shadowtechnique);
		PROP_SET_N("colourShadowR",      colourshadow.r);
		PROP_SET_N("colourShadowG",      colourshadow.g);
		PROP_SET_N("colourShadowB",      colourshadow.b);
		PROP_SET_N("farDistanceShadow",  fardistanceshadow);
	}
}




const gsProperty &gsUserDefs::getProperty(const gkString &name)
{
	return __getitem__(name.c_str());
}




void gsUserDefs::addProperty(const gsProperty &prop)
{
	if (!prop.getName().empty())
	{
		if (m_properties.find(prop.getName()) == UT_NPOS)
			m_properties.insert(prop.getName(), new gsProperty(prop));
	}
}



bool gsUserDefs::hasProperty(const gkString &name)
{
	return m_properties.find(name) != UT_NPOS;
}



const gsProperty &gsUserDefs::__getitem__(const char *name)
{
	static gsProperty *prop=0;
	UTsize pos = 0;
	if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
	{
		delete prop;
		prop = new gsProperty((const gsProperty &)(*(m_properties.at(pos))));
	}
	else
	{
		if (prop)
			delete prop;
		prop = new gsProperty();
	}
	return *prop;
}



void gsUserDefs::__setitem__(const char *name, bool  v)
{
	UTsize pos = 0;
	if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
	{
		(m_properties.at(pos))->fromBool(v);
		setValueEvent(m_properties.at(pos)->getClassRef());
	}
}



void gsUserDefs::__setitem__(const char *name, double v)
{
	UTsize pos = 0;
	if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
	{
		(m_properties.at(pos))->fromNumber(v);
		setValueEvent(m_properties.at(pos)->getClassRef());
	}
}



void gsUserDefs::__setitem__(const char *name, const gkString &v)
{
	UTsize pos = 0;
	if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
	{
		(m_properties.at(pos))->fromString(v);
		setValueEvent(m_properties.at(pos)->getClassRef());
	}
}



gsMouse::gsMouse()
	:	xpos(0), ypos(0),
	    xrel(0), yrel(0),
	    winx(0), winy(0),
	    wheel(0), moved(false)
{
}




gsMouse::~gsMouse()
{
}



void gsMouse::capture(void)
{
	if (!gkWindowSystem::getSingletonPtr()) return;

	gkMouse *mse = gkWindowSystem::getSingleton().getMouse();
	xpos    = mse->position.x;
	ypos    = mse->position.y;
	xrel    = mse->relitave.x;
	yrel    = mse->relitave.y;
	winx    = mse->winsize.x;
	winy    = mse->winsize.y;
	wheel   = mse->wheelDelta;
	moved   = mse->moved;
}


bool gsMouse::isButtonDown(gsMouseButton btn)
{
	if (!gkWindowSystem::getSingletonPtr()) return false;
	return gkWindowSystem::getSingleton().getMouse()->isButtonDown(btn);
}






gsKeyboard::gsKeyboard()
{
}


gsKeyboard::~gsKeyboard()
{
}



bool gsKeyboard::isKeyDown(int sc)
{
	if (!gkWindowSystem::getSingletonPtr()) return false;
	return gkWindowSystem::getSingleton().getKeyboard()->isKeyDown((gkScanCode)sc);
}



gsEngine::gsEngine()
{
	m_defs = 0;
	m_running = false;
	if (m_ctxOwner = ((m_engine = gkEngine::getSingletonPtr()) == 0))
		m_engine = new gkEngine();
	else
	{
		m_engine = gkEngine::getSingletonPtr();
		m_running = m_engine->isRunning();
	}

	if (m_engine)
		m_engine->addListener(this);
}



gsEngine::~gsEngine()
{
	if (m_defs)
		delete m_defs;


	for (UTsize i=0; i<m_ticks.size(); ++i)
		delete m_ticks.at(i);

	if (m_engine)
		m_engine->removeListener(this);

	if (m_ctxOwner)
	{
		m_running = false;
		delete m_engine;
	}
}




void gsEngine::connect(int evt, gsFunction func)
{
	if (evt == EVT_TICK)
		m_ticks.push_back(new gkLuaEvent(func));
}




void gsEngine::connect(int evt, gsSelf self, gsFunction method)
{
	if (evt == EVT_TICK)
		m_ticks.push_back(new gkLuaEvent(self, method));
}



void gsEngine::tick(gkScalar delta)
{
	UTsize i;
	for (i=0; i<m_ticks.size(); ++i)
	{
		gkLuaEvent *ob = m_ticks.at(i);

		ob->beginCall();
		ob->addArgument(delta);

		if (!ob->call())
		{
			m_ticks.erase(i);
			delete ob;

			if (i > 0)
			{
				i -= 1;
			}
			else 
				break;
			continue;
		}
	}
}




void gsEngine::initialize(void)
{
	if (m_ctxOwner && m_engine && !m_engine->isInitialized())
		m_engine->initialize();
}




void gsEngine::requestExit(void)
{
	if (m_engine)
		m_engine->requestExit();
}




gsUserDefs &gsEngine::getUserDefs(void)
{
	if (m_defs == 0)
		m_defs = new gsUserDefs(&m_engine->getUserDefs());
	return *m_defs;
}




gsScene* gsEngine::loadBlendFile(const gkString &name)
{
	if (m_engine && !m_running && m_ctxOwner)
	{
		if (!m_engine->isInitialized())
			gkLogMessage("gsEngine: loadBlendFile on uninitialized engine.");

		gkBlendFile *gkb = m_engine->loadBlendFile(name, gkBlendLoader::LO_ONLY_ACTIVE_SCENE, "<gkBuiltin>");
		if (!gkb)
		{
			printf("File Loading failed!\n");
			return 0;
		}

		if (gkb->getMainScene())
			return new gsScene(gkb->getMainScene());
		else
			gkLogMessage("gsEngine: no usable scenes found in blend.");
	}

	return 0;
}


gsScene *gsEngine::getActiveScene(void)
{
	if (m_engine && m_engine->isInitialized())
		return new gsScene(m_engine->getActiveScene());
	return 0;
}



void gsEngine::run(void)
{
	if (!m_running && m_engine)
	{
		m_running = true;
		m_engine->run();
	}
}




gsGameObject *gsGameObject::createNew(gkGameObject *ob)
{
	if (!ob) return 0;

	switch (ob->getType())
	{
	case GK_CAMERA:
		return new gsCamera(ob);
	case GK_LIGHT:
		return new gsLight(ob);
	case GK_ENTITY:
		return new gsEntity(ob);
	case GK_OBJECT:
		return new gsGameObject(ob);
	case GK_SKELETON:
		return new gsSkeleton(ob);
	default:
		break;
	}
	return 0;
}



gsObject::gsObject() : m_object(0)
{
}



gsObject::gsObject(gkInstancedObject *ob) : m_object(ob)
{
}



void gsObject::createInstance(void)
{
	if (m_object)
	{
		gkEngine *eng= gkEngine::getSingletonPtr();

		if (eng->isRunning())
			m_object->addCreateInstanceQueue();
		else
			m_object->createInstance();
	}
}



void gsObject::destroyInstance(void)
{
	if (m_object)
	{
		gkEngine *eng= gkEngine::getSingletonPtr();

		if (eng->isRunning())
			m_object->addDestroyInstanceQueue();
		else
			m_object->destroyInstance();
	}
}



void gsObject::reinstance(void)
{
	if (m_object)
	{
		gkEngine *eng= gkEngine::getSingletonPtr();

		if (eng->isRunning())
			m_object->addReInstanceQueue();
		else
			m_object->reinstance();
	}
}



gkString gsObject::getName(void)
{
	if (m_object)
		return m_object->getName();
	return "";
}





gsScene::gsScene()
{
}


gsScene::gsScene(gkInstancedObject *ob) : gsObject(ob)
{
}



gsScene::~gsScene()
{
}



bool gsScene::hasObject(const gkString &name)
{
	if (m_object)
		return cast<gkScene>()->hasObject(name);
	return false;
}




gsGameObject* gsScene::getObject(const gkString &name)
{
	if (m_object)
	{
		gkGameObject *gobj = cast<gkScene>()->getObject(name);
		if (gobj)
			return gsGameObject::createNew(gobj);
	}
	return 0;
}


gsGameObject* gsScene::createEmpty(const gkString &name)
{
	if (m_object)
	{
		gkScene *scene = cast<gkScene>();
		if (!scene->hasObject(name))
		{
			gkGameObject *obj = scene->createObject(name);
			return new gsGameObject(obj);
		}
	}

	return 0;
}


gsDynamicsWorld* gsScene::getDynamicsWorld(void)
{
	if (m_object)
	{
		gkScene *scene = cast<gkScene>();
		gkDynamicsWorld* world = scene->getDynamicsWorld();
		return new gsDynamicsWorld(world);
	}

	return 0;
}


gsScene* getActiveScene(void)
{
	gkEngine *eng = gkEngine::getSingletonPtr();
	if (eng && eng->isInitialized())
		return new gsScene(eng->getActiveScene());
	return 0;
}



gsArray<gsGameObject, gkGameObject> &gsScene::getObjectList(void)
{
	m_objectCache.clear();


	if (m_object)
	{
		gkScene *scene = cast<gkScene>();
		gkGameObjectSet &objs = scene->getInstancedObjects();
		gkGameObjectSet::Iterator it = objs.iterator();

		while (it.hasMoreElements())
		{
			gkGameObject *obj = it.getNext();
			m_objectCache.push(obj);
		}
	}
	return m_objectCache;
}




gsGameObject::gsGameObject()
{
}



gsGameObject::gsGameObject(gkInstancedObject *ob) : gsObject(ob)
{
}




gsVector3 gsGameObject::getPosition(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getPosition());
	return gsVector3();
}



gsVector3 gsGameObject::getRotation(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getRotation().toVector3());
	return gsVector3();
}



gsQuaternion gsGameObject::getOrientation(void)
{
	if (m_object)
		return gsQuaternion(cast<gkGameObject>()->getOrientation());
	return gsQuaternion();
}



gsVector3 gsGameObject::getScale(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getScale());
	return gsVector3();
}



gsVector3 gsGameObject::getWorldPosition(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getWorldPosition());
	return gsVector3();
}



gsVector3 gsGameObject::getWorldRotation(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getWorldRotation().toVector3());
	return gsVector3();
}



gsQuaternion gsGameObject::getWorldOrientation(void)
{
	if (m_object)
		return gsQuaternion(cast<gkGameObject>()->getWorldOrientation());
	return gsQuaternion();
}



gsVector3 gsGameObject::getLinearVelocity(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getLinearVelocity());
	return gsVector3();
}




gsVector3 gsGameObject::getAngularVelocity(void)
{
	if (m_object)
		return gsVector3(cast<gkGameObject>()->getAngularVelocity());
	return gsVector3();
}




void gsGameObject::setLinearVelocity(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setLinearVelocity(v);
}



void gsGameObject::setLinearVelocity(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setLinearVelocity(gkVector3(x,y,z));
}



void gsGameObject::setAngularVelocity(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setAngularVelocity(v);
}



void gsGameObject::setAngularVelocity(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setAngularVelocity(gkVector3(x,y,z));
}



void gsGameObject::setPosition(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setPosition(v);
}



void gsGameObject::setPosition(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setPosition(gkVector3(x,y,z));
}




void gsGameObject::setRotation(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkEuler(v));
}



void gsGameObject::setRotation(float yaw, float pitch, float roll)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkEuler(yaw, pitch, roll));
}



void gsGameObject::setOrientation(const gsQuaternion &quat)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(quat);
}



void gsGameObject::setOrientation(float w, float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkQuaternion(w, x, y, z));
}




gsGameObjectTypes gsGameObject::getType(void)
{
	if (m_object)
		return (gsGameObjectTypes)cast<gkGameObject>()->getType();
	return OB_UNKNOWN;
}




void gsGameObject::rotate(float dx, float dy, float dz)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), TRANSFORM_PARENT);
}




void gsGameObject::rotate(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(v), TRANSFORM_PARENT);
}



void gsGameObject::rotate(const gsQuaternion &v)
{
	if (m_object)
		cast<gkGameObject>()->rotate(v, TRANSFORM_PARENT);
}



void gsGameObject::rotate(float dx, float dy, float dz, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), ts);
}



void gsGameObject::rotate(const gsVector3 &v, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(v), ts);
}



void gsGameObject::rotate(const gsQuaternion &v, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(v, ts);
}



void gsGameObject::translate(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->translate(v, TRANSFORM_PARENT);
}



void gsGameObject::translate(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->translate(gkVector3(x,y,z), TRANSFORM_PARENT);
}


void gsGameObject::translate(const gsVector3 &v, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->translate(v, ts);
}


void gsGameObject::translate(float x, float y, float z, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->translate(gkVector3(x,y,z), ts);
}


void gsGameObject::scale(const gsVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->scale(v);
}



void gsGameObject::scale(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->scale(gkVector3(x,y,z));
}



void gsGameObject::yaw(float deg)
{
	if (m_object)
		cast<gkGameObject>()->yaw(gkDegree(deg), TRANSFORM_PARENT);
}




void gsGameObject::yaw(float deg, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->yaw(gkRadian(deg), ts);
}



void gsGameObject::pitch(float deg)
{
	if (m_object)
		cast<gkGameObject>()->pitch(gkRadian(deg), TRANSFORM_PARENT);
}



void gsGameObject::pitch(float deg, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->pitch(gkRadian(deg), ts);
}



void gsGameObject::roll(float deg)
{
	if (m_object)
		cast<gkGameObject>()->roll(gkRadian(deg), TRANSFORM_PARENT);
}



void gsGameObject::roll(float deg, gsTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->roll(gkRadian(deg), ts);
}



gsProperty gsGameObject::__getitem__(const gkString &prop)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			return gsProperty(var);
	}
	return gsProperty();
}



void gsGameObject::__setitem__(const gkString &prop, bool  v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(v);
	}
}



void gsGameObject::__setitem__(const gkString &prop, float v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(v);
	}
}



void gsGameObject::__setitem__(const gkString &prop, const char *v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(gkString(v));
	}
}



gsScene* gsGameObject::getScene(void)
{
	if (m_object)
		return new gsScene(cast<gkGameObject>()->getOwner());
	return 0;
}



int gsGameObject::getState(void)
{
	if (m_object)
		return cast<gkGameObject>()->getState();
	return -1;
}




bool gsGameObject::hasParent()
{
	return m_object && cast<gkGameObject>()->getParent() != 0;
}



void gsGameObject::setParent(gsGameObject *par)
{
	if (m_object && par)
	{
		gkGameObject *gobj = get();
		gkGameObject *pobj = par->get();

		if (gobj == pobj) return;

		if (!gobj->hasParent())
			gobj->setParent(pobj);
		else
		{
			if (gobj->getParent() != pobj)
			{
				gobj->getParent()->removeChild(gobj);
				gobj->setParent(pobj);
			}
		}
	}
}



void gsGameObject::setParentInPlace(gsGameObject *par)
{
	if (m_object && par)
	{
		gkGameObject *gobj = get();
		gkGameObject *pobj = par->get();

		if (gobj == pobj) return;

		if (!gobj->hasParent())
			gobj->setParentInPlace(pobj);
		else
		{
			if (gobj->getParent() != pobj)
			{
				gobj->clearParentInPlace();
				gobj->setParentInPlace(pobj);
			}
		}
	}
}



void gsGameObject::clearParent(void)
{
	if (m_object)
	{
		gkGameObject *gobj = get();

		if (gobj->hasParent())
			gobj->clearParent();
	}
}



void gsGameObject::clearParentInPlace(void)
{
	if (m_object)
	{
		gkGameObject *gobj = get();

		if (gobj->hasParent())
			gobj->clearParentInPlace();
	}
}



void gsGameObject::addChild(gsGameObject *chi)
{
	if (m_object && chi)
	{
		gkGameObject *gobj = get();
		gkGameObject *cobj = chi->get();
		if (gobj == cobj) return;

		if (!gobj->hasChild(cobj))
			gobj->addChild(cobj);
	}
}


void gsGameObject::removeChild(gsGameObject *chi)
{
	if (m_object && chi)
	{
		gkGameObject *gobj = get();
		gkGameObject *cobj = chi->get();
		if (gobj == cobj) return;


		if (gobj->hasChild(cobj))
			gobj->removeChild(cobj);
	}
}



gsGameObject* gsGameObject::getParent(void)
{
	return m_object && hasParent() ?
	       gsGameObject::createNew(cast<gkGameObject>()->getParent()) : 0;
}



void gsGameObject::enableContacts(bool v)
{
	if (m_object)
	{
		gkPhysicsController *ob = get()->getPhysicsController();
		if (ob)
			ob->enableContactProcessing(v);
	}
}



bool gsGameObject::hasContacts()
{
	if (m_object)
	{
		gkPhysicsController *ob = get()->getPhysicsController();
		if (ob)
			return !ob->getContacts().empty();
	}
	return false;
}



bool gsGameObject::hasContact(const gkString &object)
{
	if (m_object)
	{
		gkPhysicsController *ob = get()->getPhysicsController();
		if (ob)
			return ob->collidesWith(object);
	}
	return false;
}


gsEntity::gsEntity()
{
}



gsEntity::gsEntity(gkInstancedObject *ob) : gsGameObject(ob)
{
}




void gsEntity::playAction(const gkString &name, float blend)
{
	if (m_object)
		cast<gkEntity>()->playAction(name, blend);
}




gsLight::gsLight()
{
}



gsLight::gsLight(gkInstancedObject *ob) : gsGameObject(ob)
{
}




gsCamera::gsCamera()
{
}



gsCamera::gsCamera(gkInstancedObject *ob) : gsGameObject(ob)
{
}



void  gsCamera::setClipping(float start, float end)
{
	if (m_object)
		cast<gkCamera>()->setClip(gkMax(start, 0.0001f), end);
}



float gsCamera::getClipStart()
{
	if (m_object)
		return cast<gkCamera>()->getCameraProperties().m_clipstart;
	return 0;
}



float gsCamera::getClipEnd()
{
	if (m_object)
		return cast<gkCamera>()->getCameraProperties().m_clipend;
	return 0;
}



void  gsCamera::setFov(float fov)
{
	if (m_object)
		cast<gkCamera>()->setFov(gkDegree(fov));
}



float gsCamera::getFov()
{
	if (m_object)
		return cast<gkCamera>()->getCameraProperties().m_fov;
	return 0;
}



void gsCamera::makeCurrent()
{
	if (m_object)
		cast<gkCamera>()->makeCurrent();
}





gsSkeleton::gsSkeleton()
{
}



gsSkeleton::gsSkeleton(gkInstancedObject *ob) : gsGameObject(ob)
{
}




gsDebugger::gsDebugger(gsScene* sc)
	:   m_debugger(0)
{
	if (sc)
	{
		gkScene *scene = sc->cast<gkScene>();
		if (scene)
			m_debugger = scene->getDebugger();

	}
}




void gsDebugger::drawLine(const gsVector3 &from, const gsVector3 &to, const gsVector3 &color)
{
	if (m_debugger)
		m_debugger->drawLine(from, to, color);
}




void gsDebugger::drawObjectAxis(gsGameObject* ptr, float size)
{
	if (m_debugger && ptr)
	{
		gkGameObject *ob = ptr->cast<gkGameObject>();
		if (ob)
		{
			const gkVector3     &axis   = ob->getWorldPosition();
			const gkQuaternion  &ori    = ob->getOrientation();

			gkVector3 x = (ori * gkVector3(size, 0, 0));
			gkVector3 y = (ori * gkVector3(0, size, 0));
			gkVector3 z = (ori * gkVector3(0, 0, size));


			m_debugger->drawLine(axis, axis + x, gkVector3(1,0,0));
			m_debugger->drawLine(axis, axis + y, gkVector3(0,1,0));
			m_debugger->drawLine(axis, axis + z, gkVector3(0,0,1));
		}
	}
}



void gsDebugger::clear(void)
{
	if (m_debugger)
		m_debugger->clear();
}




void gsDebugPrint(const char *str)
{
	gkDebugScreen::printTo(str);
}
