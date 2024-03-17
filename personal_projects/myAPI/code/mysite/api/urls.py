from django.urls import path,include
from . import views
 
#urls de l'api
 
urlpatterns = [
    path('reviews/list/', views.ReviewList.as_view(),name='review-list'), # peu importe la requete, elle est enoyée sur les urls de api
    path('review/<int:pk>/', views.ReviewRetreive.as_view(),name='review-search'), # peu importe la requete, elle est enoyée sur les urls de api
    path('book/<int:pk>/', views.BookRetreive.as_view(),name='book-search'), # peu importe la requete, elle est enoyée sur les urls de api
    path('books/', views.BookListCreate.as_view(),name='create Book'), # peu importe la requete, elle est enoyée sur les urls de api
    path('emotions/', views.EmotionList.as_view(),name='Emotions'), # peu importe la requete, elle est enoyée sur les urls de api
    path('emotion/<int:pk>/', views.EmotionRetreive.as_view(),name='Emotions'), # peu importe la requete, elle est enoyée sur les urls de api
    #path('api/', include("api.urls")), # unquement si la requete contient /api/, elle est enoyée sur les urls de api
]
