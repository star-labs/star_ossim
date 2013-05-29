#include <ossimGui/RegistrationOverlay.h>
#include <ossimGui/RegPoint.h>
#include <ossimGui/IvtGeomTransform.h>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QtGui>
#include <iostream>


ossimGui::RegistrationOverlay::RegistrationOverlay(const ossimString& overlayId, QGraphicsScene* scene)
: OverlayBase(overlayId, scene),
  m_currentId("NS"),
  m_isControlImage(false),
  m_hasAdjParInterface(false)
{}

void ossimGui::RegistrationOverlay::reset()
{
   QList<QGraphicsItem*> items = m_scene->items();
   for (int i=0; i<items.size(); ++i)
   {
         m_scene->removeItem(items[i]);
   }

   m_isActive = false;
   m_currentId = "NS";
}

ossim_uint32 ossimGui::RegistrationOverlay::getNumPoints()const
{
   return m_scene->items().size();
}

void ossimGui::RegistrationOverlay::addPoint(const ossimDpt& scenePt, const ossimDpt& imagePt)
{
   addPoint(scenePt, imagePt, m_currentId);
}

void ossimGui::RegistrationOverlay::addPoint(const ossimDpt& scenePt, const ossimDpt& imagePt, const ossimString& id)
{
   if (id != "NS")
   {
      // Check for duplicate (changing position of point already added)
      removePoint(id);

      // Add point to scene
      ossimGui::RegPoint* pt = new ossimGui::RegPoint(scenePt, imagePt, m_overlayId, id);
      m_scene->addItem(pt);

      // Notify MultiImageDialog
      emit pointActivated(id);
   }
}


void ossimGui::RegistrationOverlay::togglePointActive(const ossimString& id)
{
   ossimGui::RegPoint* point = getRegPoint(id);
   bool currentlyActive;

   if (point != 0)
   {
      // Toggle point in layer map
      currentlyActive = point->isUsable();
      point->setUsable(!currentlyActive);
   }

   // Notify MultiImageDialog
   if (currentlyActive)
      emit pointDeactivated(id);
   else
      emit pointActivated(id);

}


void ossimGui::RegistrationOverlay::removePoint(const ossimString& id)
{
   QGraphicsItem* item = getItem(id);

   if (item != 0)
   {
      m_scene->removeItem(item);
      emit pointRemoved(id);
   }
}


bool ossimGui::RegistrationOverlay::getImgPoint(const ossimString& id,
                                                      ossimDpt& imgPt,
                                                      bool& isActive)
{
   bool isFound = false;

   ossimGui::RegPoint* point = getRegPoint(id);

   if (point != 0)
   {
      imgPt = point->getImgPos();
      isActive = point->isUsable();
      isFound = true;
   }

   return isFound;
}


ossimGui::RegPoint* ossimGui::RegistrationOverlay::getRegPoint(const ossimString& id)
{
   ossimGui::RegPoint* point = 0;

   QGraphicsItem* item = getItem(id);
   if (item != 0)
   {
      point = dynamic_cast<ossimGui::RegPoint*>(item);
   }

   return point;
}


void ossimGui::RegistrationOverlay::setVisible(const bool& visible)
{
   QList<QGraphicsItem*> items = m_scene->items();
   for (int i=0; i<items.size(); ++i)
   {
      items[i]->setVisible(visible);
   }
}


void ossimGui::RegistrationOverlay::setAsControl(const bool& controlImage)
{
   m_isControlImage = controlImage;

   QList<QGraphicsItem*> items = m_scene->items();
   for (int i=0; i<items.size(); ++i)
   {
      ossimGui::RegPoint* thisPoint = dynamic_cast<ossimGui::RegPoint*>(items[i]);
      thisPoint->setAsControlPoint(controlImage);
   }
}


void ossimGui::RegistrationOverlay::setHasAdjParInterface(const bool& hasAdjIface)
{
   m_hasAdjParInterface = hasAdjIface;
}


void ossimGui::RegistrationOverlay::setView(ossimRefPtr<IvtGeomTransform> ivtg)
{
   QList<QGraphicsItem*> items = m_scene->items();
   for (int i=0; i<items.size(); ++i)
   {
      ossimGui::RegPoint* thisPoint = dynamic_cast<ossimGui::RegPoint*>(items[i]);
      if (thisPoint)
      {
         ossimDpt imgPos = thisPoint->getImgPos();
         ossimDpt viewPos;
         ivtg->imageToView(imgPos, viewPos);
         thisPoint->setPos(viewPos.x, viewPos.y);
      }
   }
}
