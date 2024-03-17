from django.shortcuts import render
from rest_framework import generics,status
from rest_framework.response import Response
from .serializers import BlogPostserialiser,BookSerializer,ReviewSerializer,EmotionSerializer
from .models import BlogPost,Book,Review,Emotion
# Create your views here.


class BlogPostListcreate(generics.ListCreateAPIView): #vue pour créer( jouter dans la liste) une nouvelle instance de blog post
    queryset= BlogPost.objects.all() # toutes les instances de blogpost
    serializer_class= BlogPostserialiser
    
    def delete(self,request,*args,**kwargs): #implementation de delete dans la vue de Liscreate (pas présente de base)
        BlogPost.objects.all().delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
    
class BlogPostRetrieveUpdateRestore(generics.RetrieveUpdateDestroyAPIView): 
    queryset= BlogPost.objects.all() # toutes les instances de blogpost
    serializer_class= BlogPostserialiser 
    lookup_field="pk" # champs pour la recherhc(ici c'est pk= primary key soit l'id)
    
    
    
class BookListCreate(generics.ListCreateAPIView): #vue pour créer( jouter dans la liste) une nouvelle instance de blog post
    queryset= Book.objects.all() # toutes les instances de blogpost
    serializer_class= BookSerializer
    
    def delete(self,request,*args,**kwargs): #implementation de delete dans la vue de Liscreate (pas présente de base)
        BlogPost.objects.all().delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
    
    def get_queryset(self):
        auth = self.request.query_params.get('author')  # Obtenez le rating depuis les paramètres de requête
        cat = self.request.query_params.get('genre')  # Obtenez le rating depuis les paramètres de requête
        em = self.request.query_params.get('emotion')  # Obtenez le rating depuis les paramètres de requête
        res=Book.objects.all() 
        if auth is not None:
            res= res.filter(author=auth)
        if cat is not None:
            res= res.filter(genre=cat)
        if em is not None:
            res= res.filter(emotions=em)
        
        return res
    

class BookRetreive(generics.RetrieveDestroyAPIView): 
    serializer_class= BookSerializer
    queryset = Book.objects.all()
    lookup_field="pk" #les resultats de recherche se font par le champ pk
    
class ReviewList(generics.ListCreateAPIView): 
    serializer_class= ReviewSerializer

    def get_queryset(self):
        rating = self.request.query_params.get('rating')  # Obtenez le rating depuis les paramètres de requête
        if rating is not None:
            return Review.objects.filter(rating=rating)
        return Review.objects.all() 
  
    

class ReviewRetreive(generics.RetrieveDestroyAPIView): 
    serializer_class= ReviewSerializer
    queryset = Review.objects.all()
    lookup_field="pk" #les resultats de recherche se font par le champ pk
    
    
    
class EmotionList(generics.ListCreateAPIView): #vue pour créer( jouter dans la liste) une nouvelle instance de blog post
    queryset= Emotion.objects.all() # toutes les instances de blogpost
    serializer_class= EmotionSerializer
    
    
    
class EmotionRetreive(generics.RetrieveUpdateAPIView): 
    serializer_class= EmotionSerializer
    queryset = Emotion.objects.all()
    lookup_field="pk" #les resultats de recherche se font par le champ pk