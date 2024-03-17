from django.test import TestCase

# Create your tests here.
from django.test import TestCase
from rest_framework.test import APIClient
from rest_framework import status
from .models import BlogPost, Book, Review, Emotion

class APITests(TestCase):
    def setUp(self):
        # Créer des objets de test pour chaque modèle
        self.blog_post = BlogPost.objects.create(title='Test Post', content='Test content')
        self.book = Book.objects.create(title='Test Book', author='Test Author', genre='Test Genre')
        self.review = Review.objects.create(book=self.book, rating=5, comment='Test comment')
        self.emotion = Emotion.objects.create(nom_emotion='Test Emotion')

 

    def test_book_api(self):
        client = APIClient()

        # Test GET request pour la liste des livres filtrée par auteur et genre
        response = client.get('/api/books/?author=Test%20Author&genre=Test%20Genre')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test GET request pour un livre spécifique
        response = client.get(f'/api/book/{self.book.pk}/')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test POST request pour créer un nouveau livre
        data = {'title': 'New Book', 'author': 'New Author', 'genre': 'New Genre'}
        response = client.post('/api/books/', data, format='json')
        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

    def test_review_api(self):
        client = APIClient()

        # Test GET request pour la liste des reviews
        response = client.get('/api/reviews/list/')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test GET request pour une review spécifique
        response = client.get(f'/api/review/{self.review.pk}/')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test POST request pour créer une nouvelle review
        data = {'book': self.book.pk, 'rating': 4, 'comment': 'New Comment'}
        response = client.post('/api/reviews/list/', data, format='json')
        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

    def test_emotion_api(self):
        client = APIClient()

        # Test GET request pour la liste des émotions
        response = client.get('/api/emotions/')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test GET request pour une émotion spécifique
        response = client.get(f'/api/emotion/{self.emotion.pk}/')
        self.assertEqual(response.status_code, status.HTTP_200_OK)

        # Test POST request pour créer une nouvelle émotion
        data = {'nom_emotion': 'New Emotion'}
        response = client.post('/api/emotions/', data, format='json')
        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
