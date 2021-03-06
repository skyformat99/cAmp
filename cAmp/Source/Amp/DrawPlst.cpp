#include "header.h"
#include "Amp.h"
#include "..\cD3\Graphics.h"
#include "..\main\App.h"


//-------------------------------------------------------------------------------------------------------------
//  Playlist
///------------------------------------------------------------------------------------------------------------
void cAmp::DrawPlsText()
{
	if (!pls)  return;
	CFont* cf = cfont[view.cfT];
	
	///  pls Tabs text	+
	//-------------------------
	/* multi line  up,dn */
	int ytb = yB_pt+1;
	if (ytb >= view.ySize)  return;
	int ntab = vPlst.size();
	if (ntab > 0)
	{	int x,y, a = view.ofsTab;
		for (y=0; y < view.yNpt; y++)
		for (x=0; x < view.xNpt; x++)  {  if (a < ntab)  {
			cf->StrCopy(vPlst[a]->name.c_str());  //name
			int w = cf->GetWidth(), o = max(0, (xW_pt-w)/2);  //center
			cf->xmax = (x+1)*xW_pt;
			if (bShowSrch && vPlst[a]->iSrch > 0)
				cf->Fclr = D3DXCOLOR(0.4,1,0.2,1);  else  cf->Fclr = D3DXCOLOR(0.7,0.8,1,1);  // search..
			cf->Write(x*xW_pt+o, ytb+y*yH_pt);	}  a++;  }
	}	cf->xmax = view.xSize;
	
	cf = cfont[view.cfP];  CFont* cf2 = cf;
	#define  clr(r,g,b)  cf->Fclr = D3DXCOLOR(r,g,b,1)

	//  info  Total dirs, files, bookm*, size, time
	//----------------------------------------------------------------
	bool bList = pls->listLen > 0;
	if (yB_pli >= view.ySize)  return;

if (ed!=ED_nFind && ed!=ED_nTab)
{
	//  get
	DWORD  aD,aF, aSi, aTm;
	if (pls->numSel > 0)
	{	aD = 0;  aF = pls->numSel;
		aSi = pls->selSize/1000000;
		aTm = pls->selTime;  }
	else if (bAllInfo)
	{	aD= aaD;  aF= aaF;  aSi= aaSi;  aTm= aaTm;  }
	else
	{	aD = pls->allDirs;  aF = pls->allFiles;
		aSi = pls->allSize/1000000;
		aTm = pls->allTime;
	}
	//  clr
	if (pls->numSel > 0)  clr(0,1,0.9);  else
	if (bAllInfo)  clr(1,0.7,1);  else
	if (!bList)  clr(1,1,1);  else
	if (!CList::bFilInf)  clr(0.7,0.8,1);  else  clr(0.65,0.75,1);

	//  size
	char st[60];
	int sg = aSi/1000;  float fsm = aSi/1000.f;
	if (sg < 1)		sfmt(st) "%3d MB", aSi);  else
	if (sg < 10)	sfmt(st) "%4.3f GB", fsm); else
	if (sg < 100)	sfmt(st) "%4.2f GB", fsm);  else
					sfmt(st) "%.1f GB", fsm);
	
	//*L*/sfmt(cf->s) "L ofs %3d  cur %3d  Lin %3d  all %3d", plst->lOfs, plst->lCur, yLpl, plst->listLen);  cf->Write(0, yBpli);
	//*M*/sfmt(cf->s) "xm %4d ym %3d %d%d%d yMd %d %6.3f", xm,ym, bL,bR,bM, yMd, mtiv);  cf->Write(0, yBpli);
	if (aD == 0)
	{	cf->Format("%d  %s", aF, st);  cf->Write(20, yB_pli);  }
	else
	{	cf->Format("%d  %d  %s", aD, aF, st);  cf->Write(0, yB_pli);  }
	if (pls->bThrTi /*&& plst->itu < plst->listLen*/)  {
		cf->Format("%6.1f%%", 100.f*pls->itu/float(pls->listLen));  cf->Write(cf->xwr, yB_pli);  }//-

	//  total time  once, str fun..
	DWORD t = aTm, ts,tm,th,td;
	ts= t%60;  t/=60;  tm= t%60;  t/=60;  th= t%24;  td= t/24;
	
		sfmt(st) "%c%c", tm%10+'0', tm/10+'0');  cf->StrCopy(st);	if (th > 0 || td > 0) {  
	  sfmt(st) " h%c%c", th%10+'0', th>9? th/10+'0': td>0?'0':' ');  cf->StrAdd(st);	if (td > 0) {  
	sfmt(st) " d%c%c%c", td%10+'0', td>9? td/10%10+'0':' ', td>99? td/100%10+'0':' ');  cf->StrAdd(st);	 }  }
	cf->dir = -1;  cf->Write(xTm, yB_pli);  cf->dir = 1;
	
	//  num sel
	//if (pls->numSel > 0)
	//{	clr(0,1,0.9);  cf->Format("%d", pls->numSel);  cf->Write(view.xSize/2+20,yBpli);  }  //Sel:

	clr(1,1,1);  cf->dir = 1;
	cf->Fs[' '] = cf->Fs['0']/2;
}

	//  Tracks  dir, file  :time
	//--------------------------------------------------------------------------------------------------
	pTrk q = NULL;
	int j=0, xB=2, xx=0, xTab = 12, yins=-1;

	// search dir
	cf->bSl = false;
	if (!bList)	{	cf->StrWr(pls->srchPath.c_str(), 0,yB_pl);  }
	cf->bSl = true;
	
	// Tracks
	if (pls->vList.size()>0 && bList /*&& !plst->bThr..*/)
	{
		//----------------------------  Times  --------------------------------
		cf2->SetTex(0);
		int i,y, im;
		i = pls->lOfs;  y = yB_pl;  j=0;
		im = min(pls->listLen, i+yL_pl);
		//**/if (bRep1)
		for (; i < im; i++,j++)  /**/if (i < pls->vList.size())
		{	q = pls->vList[i];  if (q)  {

			///  Time
			cf2->dir = -1;  cf2->xmax = view.xSize-view.xW_plS-2;
			cf2->Fs[' '] = cf2->Fs['0'];
			xx = xTm;
			double ti = q->time;
			if (iTimeTest > 0)  // time colors test
				ti = iTimeTest == 1 ?
					pow(float(i)/60.f, 1.2f) * 800.f :
					pow(float(i)/40.f, 1.2f) * 3000.f;
			
			///  clr from time  . . . . .
			int l = vTclr.size();
			if (l > 1 && tmClrMode != 0)
			{
				float t = ti/60.f;
				int i1= 0, i2= 0;
				for (int i=1; i < vTclr.size(); ++i)
					if (t > vTclr[i-1].a)	{	i1=i2;  i2=i;	}
				
				if (tmClrMode==2)  //2 lin smooth
				{	float t2= vTclr[i2].a, t1= vTclr[i1].a, d= t2-t1, v1,v2;
					if (d > 0.f)  {  v2= (t-t1)/d;  v1= (t2-t)/d;  }
					else  {  v2= 1.f;  v1= 0.f;  }
					cf2->Fclr = D3DXCOLOR( vTclr[i1].r * v1 + vTclr[i2].r * v2,
						vTclr[i1].g * v1 + vTclr[i2].g * v2, vTclr[i1].b * v1 + vTclr[i2].b * v2, 1);
				}else  //1 last
					cf2->Fclr = D3DXCOLOR( vTclr[i2].r, vTclr[i2].g, vTclr[i2].b, 1);
			}else  //0 const
				cf2->Fclr = D3DXCOLOR(0.4,0.7,1.0,1);
			
			//  str
			if (ti > 0)  strTime(cf2->str, ti);
			else  {  xx -= xt_;  cf2->str[0]=0;  }

			bool d = q->isDir();
			if (!d)
				cf2->Write(xx, y);

			Lxm[j] = d ? cf2->xmax-20/*+*/ : cf2->xwr;  // xmax
			//cf2->StrWr("|",Lxm[j], y);

			if (i == pls->lInsPos)  yins = y;
			y += cf->Fy;
		}  }

		//----------------------------  Tracks  --------------------------------
		cf->SetTex(1);
		cf->dir = 1;
		cf->Fs[' '] = cf->Fs['0']/2;

		i = pls->lOfs;  y = yB_pl;  j=0;
		for (; i < im; i++,j++)  /**/if (i < pls->vList.size())
		{	q = pls->vList[i];  if (q)  {

			bool d = q->isDir();
			bool bPl = bPlay && i == pls->idPl;  //q == plst->tkPl;
			int xwt = Lxm[j];
			cf->xmax = xwt;
			
			///  rating
			int r = q->rate+cR0;
			if (!d)  {
				clr(0.95,0.95,0.9);
				if (r != cR0 && r >= 0 && r < chRall)  {
				cf->Format("%c", chRates[r]);  cf->Write(xB, y);  }  }

			///  dbg pls  clr
			#if 0
			bool mid = i > 0 && i < pls->vList.size()-1;
				 if (d)	               clr(0.8,0.7,0.95);  //dir
			else if (q->prev == NULL)  clr(1.0,0.3,0.2);  //1st
			else if (q->next == NULL)  clr(1.0,0.3,0.2);  //last
			else if (mid && q->next->prev->name != q->prev->next->name)					  clr(1.0,0.2,0.4);  //split
			else if (mid && q->next->name != pls->vList[i+1]->name && !q->next->isDir())  clr(1.0,0.7,0.3);  //next
 			else if (mid && q->prev->name != pls->vList[i-1]->name && !q->prev->isDir())  clr(1.0,0.7,0.3);  //prev
			else
			#endif
			///  clr
			if (bShowSrch && q->srch > 0)	clr(0.2,1,0);  // search
			else if (d)				clr(0.9,0.8,1.0);	//dir
			else if (q->dis == 1)	clr(0.4,0.6,0.9);	// dis
			else if (q->sel > 0)	clr(0.0,1.0,0.9);	// sel
			else if (i==pls->lCur)	clr(0.9,0.9,1);		// cur
			else if (bPl)			clr(1,1,1);			//play
			else					clr(0.75,0.9,1);	// norm

			///  Name
			#if 1
				//  normal
				if (CList::iDirView == 2 && d)
					cf->Format("%s", q->path.c_str());
				else
					cf->Format("%s", q->name_pls.c_str());  // offset long.. shift wheel
			#else
				//  debug list
				if (d)
					sfmt(cf->s) "%s  %s", q->name.c_str(),  pls->ll->name.c_str());
				else
					sfmt(cf->s) "%s <   %s   > %s",
						!q->prev ? "-" : q->prev->name.c_str(),  q->name.c_str(),
						!q->next ? "-" : q->next->name.c_str() );
			#endif
			
			if (CList::iDirView == 1)
				if (!q->path2.empty()) {  cf->StrAdd(" / ");  cf->StrAdd(q->path2.c_str());  }
			
			//  write
			xx = d ? xB : xB + xTab;
			int xw = xx + cf->GetWidth();
			cf->Write(xx, y);
			
			if (xw > xwt)
			{	//  more sign
				cf->xmax = view.xSize;  //|
				cf->StrWr("..", xwt, y);
			}
			y += cf->Fy;
		}  }

		///  insert pos line --
		if (alt && act && bMInWnd)
		{
			int yy = yins;
			bool shct = shift|ctrl;
			D3DCOLOR clr = shct ? D3DCOLOR_ARGB(0, 60,230,230) : D3DCOLOR_ARGB(0,100,160,210);
			if (shift)  yy = yB_pl;  else
			if (ctrl)   yy = cf->Fy*min(yL_pl,pls->listLen) + yB_pl;  else
			if (pls->lInsM == 1)  yy += cf->Fy;
			if (yins != -1 || shct)
			{	D3DRECT rIns= {0,yy-1, view.xSize,yy+1};  pDev->Clear(1, &rIns, clFl, clr, 1.f, 0);  }
		}
	}
	cf->SetTex(1);
}
