#ifndef CADCAMERAMAN_H
#define CADCAMERAMAN_H

//#ifndef __SdkCameraMan_H__
//#define __SdkCameraMan_H__

#include "OGRE/Ogre.h"
#include <limits>

#include <QMouseEvent>

namespace OgreBites
{
    enum CameraStyle   // enumerator values for different styles of camera movement
    {
        CS_FREELOOK,
        CS_ORBIT,
        CS_MANUAL
    };

    /*=============================================================================
    | Utility class for controlling the camera in samples.
    =============================================================================*/
    class QCameraMan
    {
    public:
        QCameraMan(Ogre::Camera* cam)
        : mCamera(0)
        , mTarget(0)
        , mOrbiting(false)
        , mZooming(false)
        , mTopSpeed(150)
        , mVelocity(Ogre::Vector3::ZERO)
        , mGoingForward(false)
        , mGoingBack(false)
        , mGoingLeft(false)
        , mGoingRight(false)
        , mGoingUp(false)
        , mGoingDown(false)
        , mFastMove(false)
        {

            setCamera(cam);
            setStyle(CS_FREELOOK);
        }

        virtual ~QCameraMan() {}

        /*-----------------------------------------------------------------------------
        | Swaps the camera on our camera man for another camera.
        -----------------------------------------------------------------------------*/
        virtual void setCamera(Ogre::Camera* cam)
        {
            mCamera = cam;
        }

        virtual Ogre::Camera* getCamera()
        {
            return mCamera;
        }

        /*-----------------------------------------------------------------------------
        | Sets the target we will revolve around. Only applies for orbit style.
        -----------------------------------------------------------------------------*/
        virtual void setTarget(Ogre::SceneNode* target)
        {
            if (target != mTarget)
            {
                mTarget = target;
                if(target)
                {
                    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
                    mCamera->setAutoTracking(false, mTarget);
                }
                else
                {
                    mCamera->setAutoTracking(false);
                }

            }


        }

        virtual Ogre::SceneNode* getTarget()
        {
            return mTarget;
        }

        /*-----------------------------------------------------------------------------
        | Sets the spatial offset from the target. Only applies for orbit style.
        -----------------------------------------------------------------------------*/
        virtual void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
        {
            mCamera->setPosition(mTarget->_getDerivedPosition());
            mCamera->setOrientation(mTarget->_getDerivedOrientation());
            mCamera->yaw(yaw);
            mCamera->pitch(-pitch);
            mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
        }

        /*-----------------------------------------------------------------------------
        | Sets the camera's top speed. Only applies for free-look style.
        -----------------------------------------------------------------------------*/
        virtual void setTopSpeed(Ogre::Real topSpeed)
        {
            mTopSpeed = topSpeed;
        }

        virtual Ogre::Real getTopSpeed()
        {
            return mTopSpeed;
        }

        /*-----------------------------------------------------------------------------
        | Sets the movement style of our camera man.
        -----------------------------------------------------------------------------*/
        virtual void setStyle(CameraStyle style)
        {
            if (mStyle != CS_ORBIT && style == CS_ORBIT)
            {
                setTarget(mTarget ? mTarget : mCamera->getSceneManager()->getRootSceneNode());
                mCamera->setFixedYawAxis(true);
                manualStop();
                setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);

            }
            else if (mStyle != CS_FREELOOK && style == CS_FREELOOK)
            {
                mCamera->setAutoTracking(false);
                mCamera->setFixedYawAxis(true);
            }
            else if (mStyle != CS_MANUAL && style == CS_MANUAL)
            {
                mCamera->setAutoTracking(false);
                manualStop();
            }
            mStyle = style;

        }

        virtual CameraStyle getStyle()
        {
            return mStyle;
        }

        /*-----------------------------------------------------------------------------
        | Manually stops the camera when in free-look mode.
        -----------------------------------------------------------------------------*/
        virtual void manualStop()
        {
            if (mStyle == CS_FREELOOK)
            {
                mGoingForward = false;
                mGoingBack = false;
                mGoingLeft = false;
                mGoingRight = false;
                mGoingUp = false;
                mGoingDown = false;
                mVelocity = Ogre::Vector3::ZERO;
            }
        }

        virtual bool frameRenderingQueued( float elapsed )
        {
            if (mStyle == CS_FREELOOK) {
                // build our acceleration vector based on keyboard input composite
                Ogre::Vector3 accel = Ogre::Vector3::ZERO;
                if (mGoingForward) accel += mCamera->getDirection();
                if (mGoingBack) accel -= mCamera->getDirection();
                if (mGoingRight) accel += mCamera->getRight();
                if (mGoingLeft) accel -= mCamera->getRight();
                if (mGoingUp) accel += mCamera->getUp();
                if (mGoingDown) accel -= mCamera->getUp();

                // if accelerating, try to reach top speed in a certain time
                Ogre::Real topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
                if (accel.squaredLength() != 0) {
                    accel.normalise();
                    mVelocity += accel * topSpeed * elapsed * 10;
                }
                // if not accelerating, try to stop in a certain time
                else mVelocity -= mVelocity * elapsed * 10;

                Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

                // keep camera velocity below top speed and above epsilon
                if (mVelocity.squaredLength() > topSpeed * topSpeed) {
                    mVelocity.normalise();
                    mVelocity *= topSpeed;
                }
                else if (mVelocity.squaredLength() < tooSmall * tooSmall)
                    mVelocity = Ogre::Vector3::ZERO;

                //std::cout << "Velocity " << mVelocity << " accel " << accel << " elapsed " << elapsed << " camera direction " << mCamera->getDirection() << std::endl;

                if (mVelocity != Ogre::Vector3::ZERO) mCamera->move( mVelocity * elapsed );
            }

            return true;
        }

        /*-----------------------------------------------------------------------------
        | Processes key presses for free-look style movement.
        -----------------------------------------------------------------------------*/
        virtual void injectKeyDown(const QKeyEvent& evt)
        {
            if (mStyle == CS_FREELOOK) {
                if (evt.key() == Qt::Key_W || evt.key() == Qt::Key_Up) mGoingForward = true;
                else if (evt.key() == Qt::Key_S || evt.key() == Qt::Key_Down) mGoingBack = true;
                else if (evt.key() == Qt::Key_A || evt.key() == Qt::Key_Left) mGoingLeft = true;
                else if (evt.key() == Qt::Key_D || evt.key() == Qt::Key_Right) mGoingRight = true;
                else if (evt.key() == Qt::Key_PageUp) mGoingUp = true;
                else if (evt.key() == Qt::Key_PageDown) mGoingDown = true;
                else if (evt.key() == Qt::Key_Shift) mFastMove = true;
            }
        }

        /*-----------------------------------------------------------------------------
          | Processes key releases for free-look style movement.
          -----------------------------------------------------------------------------*/
        virtual void injectKeyUp(const QKeyEvent& evt)
        {
            if (mStyle == CS_FREELOOK) {
                if (evt.key() == Qt::Key_W || evt.key() == Qt::Key_Up) mGoingForward = false;
                else if (evt.key() == Qt::Key_S || evt.key() == Qt::Key_Down) mGoingBack = false;
                else if (evt.key() == Qt::Key_A || evt.key() == Qt::Key_Left) mGoingLeft = false;
                else if (evt.key() == Qt::Key_D || evt.key() == Qt::Key_Right) mGoingRight = false;
                else if (evt.key() == Qt::Key_PageUp) mGoingUp = false;
                else if (evt.key() == Qt::Key_PageDown) mGoingDown = false;
                else if (evt.key() == Qt::Key_Shift) mFastMove = false;
            }
        }


        /*-----------------------------------------------------------------------------
        | Processes mouse movement differently for each style.
        -----------------------------------------------------------------------------*/
#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS) || (OGRE_PLATFORM == OGRE_PLATFORM_ANDROID)
        //virtual void injectMouseMove(const OIS::MultiTouchEvent& evt)
#else
        virtual void injectMouseMove(const QMouseEvent& evt)
#endif
        {
            if ( previousMousePosition.x() == 0 && previousMousePosition.y() == 0 ) {
                /// First mouse move event - only initialize
                previousMousePosition = evt.pos();
                return;
            }

            /// calculate relative move and backup previous position
            QPoint rel = evt.pos() - previousMousePosition;
            previousMousePosition = evt.pos();

            if (mStyle == CS_ORBIT) {
                Ogre::Real dist = (mCamera->getPosition() - mTarget->_getDerivedPosition()).length();

                if (mOrbiting) {   // yaw around the target, and pitch locally
                    mCamera->setPosition(mTarget->_getDerivedPosition());

                    mCamera->yaw(Ogre::Degree(-rel.x() * 0.25f));
                    mCamera->pitch(Ogre::Degree(-rel.y() * 0.25f));

                    mCamera->moveRelative(Ogre::Vector3(0, 0, dist));

                    // don't let the camera go over the top or around the bottom of the target
                }
                else if (mZooming) {  // move the camera toward or away from the target
                    // the further the camera is, the faster it moves
                    mCamera->moveRelative(Ogre::Vector3(0, 0, rel.y() * 0.004f * dist));
                }
#if 0
                else if (evt.state.Z.rel != 0) {  // move the camera toward or away from the target
                    // the further the camera is, the faster it moves
                    mCamera->moveRelative(Ogre::Vector3(0, 0, -evt.state.Z.rel * 0.0008f * dist));
                }
#endif
            }
            else if ( mStyle == CS_FREELOOK ) {
                mCamera->yaw( Ogre::Degree( -rel.x() * 0.15f ) );
                mCamera->pitch( Ogre::Degree( -rel.y() * 0.15f ) );
            }
        }

        /*-----------------------------------------------------------------------------
        | Processes mouse presses. Only applies for orbit style.
        | Left button is for orbiting, and right button is for zooming.
        -----------------------------------------------------------------------------*/
#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS) || (OGRE_PLATFORM == OGRE_PLATFORM_ANDROID)
        virtual void injectMouseDown(const OIS::MultiTouchEvent& evt)
        {
            if (mStyle == CS_ORBIT)
            {
                mOrbiting = true;
            }
        }
#else
        virtual void injectMouseDown(const QMouseEvent& evt)
        {
            /// reset previous position
            previousMousePosition = QPoint( 0, 0 );

            if (mStyle == CS_ORBIT) {
                if (evt.button() == Qt::LeftButton) mOrbiting = true;
                else if (evt.button() == Qt::RightButton) mZooming = true;
            }
        }
#endif

        /*-----------------------------------------------------------------------------
        | Processes mouse releases. Only applies for orbit style.
        | Left button is for orbiting, and right button is for zooming.
        -----------------------------------------------------------------------------*/
#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS) || (OGRE_PLATFORM == OGRE_PLATFORM_ANDROID)
        virtual void injectMouseUp(const OIS::MultiTouchEvent& evt)
        {
            if (mStyle == CS_ORBIT)
            {
                mOrbiting = false;
            }
        }
#else
        virtual void injectMouseUp(const QMouseEvent& evt)
        {
            if (mStyle == CS_ORBIT) {
                if (evt.button() == Qt::LeftButton) mOrbiting = false;
                else if (evt.button() == Qt::RightButton) mZooming = false;
            }
        }
#endif

    protected:

        Ogre::Camera* mCamera;
        CameraStyle mStyle;
        Ogre::SceneNode* mTarget;
        bool mOrbiting;
        bool mZooming;
        Ogre::Real mTopSpeed;
        Ogre::Vector3 mVelocity;
        bool mGoingForward;
        bool mGoingBack;
        bool mGoingLeft;
        bool mGoingRight;
        bool mGoingUp;
        bool mGoingDown;
        bool mFastMove;

        QPoint previousMousePosition;
    };
}

//#endif

#endif // CADCAMERAMAN_H
