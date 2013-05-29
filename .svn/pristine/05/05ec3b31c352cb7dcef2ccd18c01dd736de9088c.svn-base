//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description:
// 
// Class definition for ossimQtHistogramWidget.
// 
// Derived from QWidget.
// 
// A QT Widget for displaying histograms.
//
//*************************************************************************
// $Id: ossimQtHistogramWidget.cpp 12805 2008-05-07 19:41:34Z gpotts $

#include <ossimGui/HistogramWidget.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/base/ossimHistogram.h>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

const int WIDTH  = 258;
const int HEIGHT = 130;


ossimGui::HistogramWidget::HistogramWidget(QWidget* parent,
                                            Qt::WindowFlags f)
   :
      QFrame(parent, f),
m_showClipPointsFlag(false)
{
   setMinimumWidth(WIDTH);
   setMinimumHeight(HEIGHT);
   m_readOnly = false;
//   setMaximumWidth(WIDTH);
//   setMaximumHeight(HEIGHT);
   //setBackgroundColor(Qt::white);
   // paintHistogram(0);
}

ossimGui::HistogramWidget::~HistogramWidget()
{
}

void ossimGui::HistogramWidget::setHistogram(ossimHistogram* histogram)
{
   m_histogram = histogram;
   if(m_histogram.valid())
   {
      updateScaledHistogram();
   }
   else 
   {
      m_scaledHistogram = 0;
   }

   update();
}

void	ossimGui::HistogramWidget::resizeEvent ( QResizeEvent * event )
{
   QWidget::resizeEvent(event);
   updateScaledHistogram();
}

void ossimGui::HistogramWidget::setPenetration(double minValue, double maxValue)
{
   m_minFraction = minValue;
   m_maxFraction = maxValue;
   
   if(m_histogram.valid())
   {   
      update();
      emit clipPenetrationsAdjusted(m_minFraction, m_maxFraction);
   }
}

void ossimGui::HistogramWidget::setClipPoints(double minValue, double maxValue)
{
   if(m_histogram.valid())
   {
      m_minFraction = m_histogram->getLowFractionFromValue(ossim::min(minValue, maxValue));
      m_maxFraction = m_histogram->getHighFractionFromValue(ossim::max(minValue, maxValue));
      update();
      emit clipPenetrationsAdjusted(m_histogram->LowClipVal(m_minFraction), m_histogram->LowClipVal(m_maxFraction));
   }
}

void ossimGui::HistogramWidget::getClipPoints(double& minValue, double& maxValue)
{
   minValue = m_histogram->LowClipVal(m_minFraction);
   maxValue = m_histogram->HighClipVal(m_maxFraction);
}

void  ossimGui::HistogramWidget::setShowClipPointsFlag(bool flag)
{
   bool updateFlag = flag != m_showClipPointsFlag;
   m_showClipPointsFlag = flag;
   
   if(updateFlag) update();
}



void ossimGui::HistogramWidget::updateScaledHistogram()
{
   if(m_histogram.valid())
   {
      ossim_uint32 w = width();
      ossim_uint32 h = height();
      float minValue = floor(m_histogram->GetMinVal());
      float maxValue = ceil(m_histogram->GetMaxVal());
      float maxBins = m_histogram->GetRes();
      m_scaledHistogram = new ossimHistogram(w, minValue, maxValue);
      float* sumCounts = m_scaledHistogram->GetCounts();
      ossim_uint32 binIdx = 0;
      float delta = (maxValue - minValue)/maxBins;
      ossim_int32 sumIndex = 0;
      float value = 0.0;
      memset(sumCounts, '\0', sizeof(float)*w);
      
      /*
      for(binIdx = 0; binIdx < maxBins;++binIdx)
      {
         value = minValue+delta*binIdx;
         sumIndex = m_scaledHistogram->GetIndex(value);
         if(sumIndex >= 0)
         {
            sumCounts[sumIndex] += m_histogram->GetCount(value);
         }
      }
       */
      ossim_uint32 idx = 0;
      ossim_float64 colWidth  = m_scaledHistogram->GetBucketSize();
      for(idx = 0; idx < w; ++idx)
      {
         ossim_float64 tempMin = minValue + idx*colWidth;
         sumCounts[idx] += m_histogram->ComputeArea(tempMin, tempMin +colWidth); 
      }
      m_scaledHistogram = m_scaledHistogram->fillInteriorEmptyBins();
      m_scaledNormalizer = m_scaledHistogram->GetMaxCount();
#if 0
      float sumMaxCount= m_scaledHistogram->GetMaxCount();
      sumCounts = m_scaledHistogram->GetCounts();
      
      
      int height = 0;
      if(sumMaxCount > 0.0)
      {
         ossim_uint32 col;
         for (col = 0; col < w; ++col)
         {
            sumCounts[col] = ossim::round<int>((sumCounts[col]/sumMaxCount)*h);
         }
      }
#endif
   }
}

void ossimGui::HistogramWidget::paintEvent(QPaintEvent* /* event */)
   
{
   const int H = height();
   QPainter p(this);
   QPen p1(Qt::black, 1);
   if (m_scaledHistogram.valid())
   {
     // p.drawRect(QRect(0,0,width()-1,H-1));
     float* sumCounts = m_scaledHistogram->GetCounts();
      ossim_int32 col;
      ossim_int32 res = m_scaledHistogram->GetRes();
      for (col = 0; col <res; ++col)
      {
         p.drawLine(col, H, col, H-(sumCounts[col]/m_scaledNormalizer)*H);
      }
      if(m_showClipPointsFlag)
      {
         double clipMinxValue = m_scaledHistogram->GetIndex(m_scaledHistogram->LowClipVal(m_minFraction));
         double clipMaxxValue = m_scaledHistogram->GetIndex(m_scaledHistogram->HighClipVal(m_maxFraction));
         p.save();
         QBrush alphaBrush(p.brush());
         alphaBrush.setColor(QColor(255,255,255,128));
         alphaBrush.setStyle(Qt::SolidPattern);
         p.setBrush(alphaBrush);
         QSize thisSize = size();
         double clipW = fabs(clipMaxxValue-clipMinxValue);
         p.drawRect(QRect(clipMinxValue,0,clipW,H));
         p.restore();
      }
   }
   
}

double ossimGui::HistogramWidget::getMaxBinCount()
{
   return 0.0;
}

void ossimGui::HistogramWidget::adjustClips(const QPoint& point)
{
   if(m_readOnly) return;
   if(point.x() > width()||point.x() < 0) return;
   if(m_scaledHistogram.valid())
   {
      double clipMinxIndex = m_scaledHistogram->GetIndex(m_scaledHistogram->LowClipVal(m_minFraction));
      double clipMaxxIndex = m_scaledHistogram->GetIndex(m_scaledHistogram->HighClipVal(m_maxFraction));
     
      double center = (clipMinxIndex + clipMaxxIndex)*0.5;
      double x = point.x();
      double binSize = m_scaledHistogram->GetBucketSize();
      double value = m_scaledHistogram->LowClipVal(0) + x*binSize;
      if(x < center)
      {
         setPenetration(m_scaledHistogram->getLowFractionFromValue(value), m_maxFraction);
      }
      else if(x > center)
      {
         setPenetration(m_minFraction, m_scaledHistogram->getHighFractionFromValue(value));
      }
   }
}


void ossimGui::HistogramWidget::mousePressEvent(QMouseEvent *e)
{
   if(e->buttons()&Qt::LeftButton)
   {
      adjustClips(e->pos());
   }
}

void ossimGui::HistogramWidget::mouseMoveEvent(QMouseEvent *e)
{
   if(e->buttons()&Qt::LeftButton)
   {
      adjustClips(e->pos());
   }
}

